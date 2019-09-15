#include "Synthesizer.h"

#include <QDebug>

#include <math.h>

#include "NoiseGenerator.h"
#include "Sound.h"

static const qreal PI = 3.14159265;

static const qreal MASTER_VOL = 0.05;

static const int NOISE_SAMPLE_COUNT = 32;

inline qreal ramp(qreal x, qreal x1, qreal x2, qreal y1, qreal y2) {
    qreal k = (x - x1) / (x2 - x1); // k goes from 0 to 1
    return y1 + k * (y2 - y1);
}

class WaveFormGenerator {
public:
    WaveFormGenerator() : mNoiseGenerator(NOISE_SAMPLE_COUNT) {
    }

    void setSound(const Sound* sound) {
        mSound = sound;
        mNoiseGenerator.reset();
    }

    qreal generate(int phase, int period) {
        qreal fp = qreal(phase) / period;

        switch (mSound->waveForm()) {
        case WaveForm::Square:
            return fp < mSquareDuty ? 0.5 : -0.5;
        case WaveForm::Sawtooth:
            return 1.0 - fp * 2;
        case WaveForm::Sine:
            return sin(fp * 2 * PI);
        case WaveForm::Noise:
            return mNoiseGenerator.get(fp);
        case WaveForm::Triangle:
            return fp < 0.5 ? ramp(fp, 0, 0.5, -1, 1) : ramp(fp, 0.5, 1, 1, -1);
        }
        Q_UNREACHABLE();
    }

    void onResetSample() {
        mSquareDuty = 0.5 - mSound->squareDuty() * 0.5;
    }

    void update() {
        if (mSound->waveForm() != WaveForm::Square) {
            return;
        }
        mSquareDuty = qBound(0.0, mSquareDuty - mSound->dutySweep() * 0.00005, 0.5);
    }

private:
    const Sound* mSound = nullptr;
    qreal mSquareDuty = 0;
    NoiseGenerator mNoiseGenerator;
};

Synthesizer::SynthStrategy::~SynthStrategy() {
}

Synthesizer::Synthesizer() : mSound(new Sound), mWaveFormGenerator(new WaveFormGenerator) {
}

Synthesizer::~Synthesizer() {
}

void Synthesizer::init(const Sound* sound) {
    mSound->fromOther(sound);
    mWaveFormGenerator->setSound(sound);
    start();
}

void Synthesizer::resetSample(bool restart) {
    fperiod = 100.0 / (mSound->baseFrequency() * mSound->baseFrequency() + 0.001);
    fmaxperiod = 100.0 / (mSound->minFrequency() * mSound->minFrequency() + 0.001);
    fslide = 1.0 - pow(mSound->slide(), 3.0) * 0.01;
    fdslide = -pow(mSound->deltaSlide(), 3.0) * 0.000001;
    mWaveFormGenerator->onResetSample();
    if (mSound->changeAmount() >= 0.0) {
        arp_mod = 1.0 - pow(mSound->changeAmount(), 2.0) * 0.9;
    } else {
        arp_mod = 1.0 + pow(mSound->changeAmount(), 2.0) * 10.0;
    }
    arp_time = 0;
    arp_limit = int(pow(1.0 - mSound->changeSpeed(), 2.0) * 20000 + 32);
    if (mSound->changeSpeed() == 1.0) {
        arp_limit = 0;
    }
    if (!restart) {
        // reset filter
        fltp = 0.0;
        fltdp = 0.0;
        fltw = pow(mSound->lpFilterCutoff(), 3.0) * 0.1;
        fltw_d = 1.0 + mSound->lpFilterCutoffSweep() * 0.0001;
        fltdmp = 5.0 / (1.0 + pow(mSound->lpFilterResonance(), 2.0) * 20.0) * (0.01 + fltw);
        if (fltdmp > 0.8) {
            fltdmp = 0.8;
        }
        fltphp = 0.0;
        flthp = pow(mSound->hpFilterCutoff(), 2.0) * 0.1;
        flthp_d = 1.0 + mSound->hpFilterCutoffSweep() * 0.0003;
        // reset vibrato
        vib_phase = 0.0;
        vib_speed = pow(mSound->vibratoSpeed(), 2.0) * 0.01;
        vib_amp = mSound->vibratoDepth() * 0.5;
        // reset envelope
        env_vol = 0.0;
        env_stage = Attack;
        env_time = 0;
        env_length[Attack] = int(mSound->attackTime() * mSound->attackTime() * 100000.0);
        env_length[Sustain] = int(mSound->sustainTime() * mSound->sustainTime() * 100000.0);
        env_length[Decay] = int(mSound->decayTime() * mSound->decayTime() * 100000.0);

        fphase = pow(mSound->phaserOffset(), 2.0) * 1020.0;
        if (mSound->phaserOffset() < 0.0) {
            fphase = -fphase;
        }
        fdphase = pow(mSound->phaserSweep(), 2.0) * 1.0;
        if (mSound->phaserSweep() < 0.0) {
            fdphase = -fdphase;
        }
        ipp = 0;
        for (int i = 0; i < PHASER_BUFFER_LENGTH; i++) {
            phaser_buffer[i] = 0.0;
        }

        rep_time = 0;
        rep_limit = int(pow(1.0 - mSound->repeatSpeed(), 2.0) * 20000 + 32);
        if (mSound->repeatSpeed() == 0.0) {
            rep_limit = 0;
        }
    }
}

void Synthesizer::start() {
    phase = 0;
    resetSample(false);
}

bool Synthesizer::synthSample(int length, SynthStrategy* strategy) {
    for (int i = 0; i < length; i++) {
        rep_time++;
        if (rep_limit != 0 && rep_time >= rep_limit) {
            rep_time = 0;
            resetSample(true);
        }

        // frequency envelopes/arpeggios
        arp_time++;
        if (arp_limit != 0 && arp_time >= arp_limit) {
            arp_limit = 0;
            fperiod *= arp_mod;
        }
        fslide += fdslide;
        fperiod *= fslide;
        if (fperiod > fmaxperiod) {
            fperiod = fmaxperiod;
            if (mSound->minFrequency() > 0.0) {
                return false;
            }
        }
        qreal rfperiod = fperiod;
        if (vib_amp > 0.0) {
            vib_phase += vib_speed;
            rfperiod = fperiod * (1.0 + sin(vib_phase) * vib_amp);
        }
        int period = std::max(int(rfperiod), 8);
        mWaveFormGenerator->update();
        // volume envelope
        env_time++;
        if (env_time > env_length[env_stage]) {
            if (env_stage == Decay) {
                return false;
            }
            env_time = 0;
            env_stage = EnvelopStage(int(env_stage) + 1);
        }
        switch (env_stage) {
        case Attack:
            env_vol = qreal(env_time) / env_length[Attack];
            break;
        case Sustain:
            env_vol = 1.0
                      + pow(1.0 - qreal(env_time) / env_length[Sustain], 1.0) * 2.0
                            * mSound->sustainPunch();
            break;
        case Decay:
            env_vol = 1.0 - qreal(env_time) / env_length[Decay];
            break;
        }

        // phaser step
        fphase += fdphase;
        int iphase = std::min(abs(int(fphase)), PHASER_BUFFER_LENGTH - 1);

        if (flthp_d != 0.0) {
            flthp *= flthp_d;
            if (flthp < 0.00001) {
                flthp = 0.00001;
            }
            if (flthp > 0.1) {
                flthp = 0.1;
            }
        }

        qreal ssample = 0.0;
        for (int si = 0; si < 8; si++) { // 8x supersampling
            phase++;
            if (phase >= period) {
                phase %= period;
            }
            // base waveform
            qreal sample = mWaveFormGenerator->generate(phase, period);

            // lp filter
            qreal pp = fltp;
            fltw = qBound(0.0, fltw * fltw_d, 0.1);
            if (mSound->lpFilterCutoff() != 1.0) {
                fltdp += (sample - fltp) * fltw;
                fltdp -= fltdp * fltdmp;
            } else {
                fltp = sample;
                fltdp = 0.0;
            }
            fltp += fltdp;
            // hp filter
            fltphp += fltp - pp;
            fltphp -= fltphp * flthp;
            sample = fltphp;
            // phaser
            phaser_buffer[ipp % PHASER_BUFFER_LENGTH] = sample;
            sample += phaser_buffer[(ipp - iphase + PHASER_BUFFER_LENGTH) % PHASER_BUFFER_LENGTH];
            ipp = (ipp + 1) % PHASER_BUFFER_LENGTH;
            // final accumulation and envelope application
            ssample += sample * env_vol;
        }
        ssample = ssample / 8 * MASTER_VOL;

        ssample *= 2.0 * mSound->volume();

        strategy->write(ssample);
    }
    return true;
}
