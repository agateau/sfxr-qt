#include "synthesizer.h"

#include <QTimer>
#include <QUrl>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "sound.h"

static const float PI = 3.14159265f;

static const float MASTER_VOL = 0.05f;

static const int SCHEDULED_PLAY_DELAY = 200;

inline int rnd(int n) {
    return rand() % (n + 1);
}

inline float frnd(float range) {
    return (float)rnd(10000) / 10000 * range;
}

Synthesizer::SynthStrategy::~SynthStrategy() {
}

class BufferStrategy : public Synthesizer::SynthStrategy {
public:
    BufferStrategy(float* buffer)
        : buffer(buffer) {
    }
    void write(float ssample) override {
        if (ssample > 1.0f) {
            ssample = 1.0f;
        }
        if (ssample < -1.0f) {
            ssample = -1.0f;
        }
        *buffer++ = ssample;
    }
private:
    float* buffer;
};

class WavExportStrategy : public Synthesizer::SynthStrategy {
public:
    FILE* file;
    int file_sampleswritten;
    float filesample = 0.0f;
    int fileacc = 0;
    int wav_bits = 16;
    int wav_freq = 44100;

    void write(float sample) override;
};

void WavExportStrategy::write(float ssample) {
    // quantize depending on format
    // accumulate/count to accomodate variable sample rate?
    ssample *= 4.0f; // arbitrary gain to get reasonable output volume...
    if (ssample > 1.0f) {
        ssample = 1.0f;
    }
    if (ssample < -1.0f) {
        ssample = -1.0f;
    }
    filesample += ssample;
    fileacc++;
    if (wav_freq == 44100 || fileacc == 2) {
        filesample /= fileacc;
        fileacc = 0;
        if (wav_bits == 16) {
            short isample = (short)(filesample * 32000);
            fwrite(&isample, 1, 2, file);
        } else {
            unsigned char isample = (unsigned char)(filesample * 127 + 128);
            fwrite(&isample, 1, 1, file);
        }
        filesample = 0.0f;
    }
    file_sampleswritten++;
}

Synthesizer::Synthesizer(QObject* parent)
    : QObject(parent)
    , mPlayTimer(new QTimer(this)) {
    mPlayTimer->setInterval(SCHEDULED_PLAY_DELAY);
    mPlayTimer->setSingleShot(true);
    connect(mPlayTimer, &QTimer::timeout, this, &Synthesizer::play);
}

Sound* Synthesizer::sound() const {
    return mSound;
}

void Synthesizer::setSound(Sound* value) {
    if (mSound == value) {
        return;
    }
    if (mSound) {
        disconnect(mSound, 0, this, 0);
    }
    mSound = value;

    connect(mSound, &Sound::waveTypeChanged, this, &Synthesizer::schedulePlay);

    connect(mSound, &Sound::attackTimeChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::sustainTimeChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::sustainPunchChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::decayTimeChanged, this, &Synthesizer::schedulePlay);

    connect(mSound, &Sound::baseFrequencyChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::minFrequencyChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::slideChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::deltaSlideChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::vibratoDepthChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::vibratoSpeedChanged, this, &Synthesizer::schedulePlay);

    connect(mSound, &Sound::changeAmountChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::changeSpeedChanged, this, &Synthesizer::schedulePlay);

    connect(mSound, &Sound::squareDutyChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::dutySweepChanged, this, &Synthesizer::schedulePlay);

    connect(mSound, &Sound::repeatSpeedChanged, this, &Synthesizer::schedulePlay);

    connect(mSound, &Sound::phaserOffsetChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::phaserSweepChanged, this, &Synthesizer::schedulePlay);

    connect(mSound, &Sound::lpFilterCutoffChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::lpFilterCutoffSweepChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::lpFilterResonanceChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::hpFilterCutoffChanged, this, &Synthesizer::schedulePlay);
    connect(mSound, &Sound::hpFilterCutoffSweepChanged, this, &Synthesizer::schedulePlay);

    soundChanged(value);
}

Synthesizer::~Synthesizer() {
}

void Synthesizer::resetSample(bool restart) {
    if (!restart) {
        phase = 0;
    }
    fperiod = 100.0 / (mSound->baseFrequency() * mSound->baseFrequency() + 0.001);
    period = (int)fperiod;
    fmaxperiod = 100.0 / (mSound->minFrequency() * mSound->minFrequency() + 0.001);
    fslide = 1.0 - pow((double)mSound->slide(), 3.0) * 0.01;
    fdslide = -pow((double)mSound->deltaSlide(), 3.0) * 0.000001;
    square_duty = 0.5f - mSound->squareDuty() * 0.5f;
    square_slide = -mSound->dutySweep() * 0.00005f;
    if (mSound->changeAmount() >= 0.0f) {
        arp_mod = 1.0 - pow((double)mSound->changeAmount(), 2.0) * 0.9;
    } else {
        arp_mod = 1.0 + pow((double)mSound->changeAmount(), 2.0) * 10.0;
    }
    arp_time = 0;
    arp_limit = (int)(pow(1.0f - mSound->changeSpeed(), 2.0f) * 20000 + 32);
    if (mSound->changeSpeed() == 1.0f) {
        arp_limit = 0;
    }
    if (!restart) {
        // reset filter
        fltp = 0.0f;
        fltdp = 0.0f;
        fltw = pow(mSound->lpFilterCutoff(), 3.0f) * 0.1f;
        fltw_d = 1.0f + mSound->lpFilterCutoffSweep() * 0.0001f;
        fltdmp = 5.0f / (1.0f + pow(mSound->lpFilterResonance(), 2.0f) * 20.0f) * (0.01f + fltw);
        if (fltdmp > 0.8f) {
            fltdmp = 0.8f;
        }
        fltphp = 0.0f;
        flthp = pow(mSound->hpFilterCutoff(), 2.0f) * 0.1f;
        flthp_d = 1.0 + mSound->hpFilterCutoffSweep() * 0.0003f;
        // reset vibrato
        vib_phase = 0.0f;
        vib_speed = pow(mSound->vibratoSpeed(), 2.0f) * 0.01f;
        vib_amp = mSound->vibratoDepth() * 0.5f;
        // reset envelope
        env_vol = 0.0f;
        env_stage = 0;
        env_time = 0;
        env_length[0] = (int)(mSound->attackTime() * mSound->attackTime() * 100000.0f);
        env_length[1] = (int)(mSound->sustainTime() * mSound->sustainTime() * 100000.0f);
        env_length[2] = (int)(mSound->decayTime() * mSound->decayTime() * 100000.0f);

        fphase = pow(mSound->phaserOffset(), 2.0f) * 1020.0f;
        if (mSound->phaserOffset() < 0.0f) {
            fphase = -fphase;
        }
        fdphase = pow(mSound->phaserSweep(), 2.0f) * 1.0f;
        if (mSound->phaserSweep() < 0.0f) {
            fdphase = -fdphase;
        }
        iphase = abs((int)fphase);
        ipp = 0;
        for (int i = 0; i < 1024; i++) {
            phaser_buffer[i] = 0.0f;
        }

        for (int i = 0; i < 32; i++) {
            noise_buffer[i] = frnd(2.0f) - 1.0f;
        }

        rep_time = 0;
        rep_limit = (int)(pow(1.0f - mSound->repeatSpeed(), 2.0f) * 20000 + 32);
        if (mSound->repeatSpeed() == 0.0f) {
            rep_limit = 0;
        }
    }
}

void Synthesizer::play() {
    resetSample(false);
    playing_sample = true;
}

void Synthesizer::synthSample(int length, SynthStrategy* strategy) {
    for (int i = 0; i < length; i++) {
        if (!playing_sample) {
            break;
        }

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
            if (mSound->minFrequency() > 0.0f) {
                playing_sample = false;
            }
        }
        float rfperiod = fperiod;
        if (vib_amp > 0.0f) {
            vib_phase += vib_speed;
            rfperiod = fperiod * (1.0 + sin(vib_phase) * vib_amp);
        }
        period = (int)rfperiod;
        if (period < 8) {
            period = 8;
        }
        square_duty += square_slide;
        if (square_duty < 0.0f) {
            square_duty = 0.0f;
        }
        if (square_duty > 0.5f) {
            square_duty = 0.5f;
        }
        // volume envelope
        env_time++;
        if (env_time > env_length[env_stage]) {
            env_time = 0;
            env_stage++;
            if (env_stage == 3) {
                playing_sample = false;
            }
        }
        if (env_stage == 0) {
            env_vol = (float)env_time / env_length[0];
        }
        if (env_stage == 1) {
            env_vol = 1.0f + pow(1.0f - (float)env_time / env_length[1], 1.0f) * 2.0f * mSound->sustainPunch();
        }
        if (env_stage == 2) {
            env_vol = 1.0f - (float)env_time / env_length[2];
        }

        // phaser step
        fphase += fdphase;
        iphase = abs((int)fphase);
        if (iphase > 1023) {
            iphase = 1023;
        }

        if (flthp_d != 0.0f) {
            flthp *= flthp_d;
            if (flthp < 0.00001f) {
                flthp = 0.00001f;
            }
            if (flthp > 0.1f) {
                flthp = 0.1f;
            }
        }

        float ssample = 0.0f;
        for (int si = 0; si < 8; si++) { // 8x supersampling
            float sample = 0.0f;
            phase++;
            if (phase >= period) {
//              phase=0;
                phase %= period;
                if (mSound->waveType() == 3)
                    for (int i = 0; i < 32; i++) {
                        noise_buffer[i] = frnd(2.0f) - 1.0f;
                    }
            }
            // base waveform
            float fp = (float)phase / period;
            switch (mSound->waveType()) {
            case 0: // square
                if (fp < square_duty) {
                    sample = 0.5f;
                } else {
                    sample = -0.5f;
                }
                break;
            case 1: // sawtooth
                sample = 1.0f - fp * 2;
                break;
            case 2: // sine
                sample = (float)sin(fp * 2 * PI);
                break;
            case 3: // noise
                sample = noise_buffer[phase * 32 / period];
                break;
            }
            // lp filter
            float pp = fltp;
            fltw *= fltw_d;
            if (fltw < 0.0f) {
                fltw = 0.0f;
            }
            if (fltw > 0.1f) {
                fltw = 0.1f;
            }
            if (mSound->lpFilterCutoff() != 1.0f) {
                fltdp += (sample - fltp) * fltw;
                fltdp -= fltdp * fltdmp;
            } else {
                fltp = sample;
                fltdp = 0.0f;
            }
            fltp += fltdp;
            // hp filter
            fltphp += fltp - pp;
            fltphp -= fltphp * flthp;
            sample = fltphp;
            // phaser
            phaser_buffer[ipp & 1023] = sample;
            sample += phaser_buffer[(ipp - iphase + 1024) & 1023];
            ipp = (ipp + 1) & 1023;
            // final accumulation and envelope application
            ssample += sample * env_vol;
        }
        ssample = ssample / 8 * MASTER_VOL;

        ssample *= 2.0f * mSound->volume();

        strategy->write(ssample);
    }
}

bool Synthesizer::exportWav(const QUrl& url) {
    QString path = url.path();
    FILE* foutput = fopen(path.toLocal8Bit().constData(), "wb");
    if (!foutput) {
        return false;
    }
    WavExportStrategy wav;
    // write wav header
    unsigned int dword = 0;
    unsigned short word = 0;
    fwrite("RIFF", 4, 1, foutput); // "RIFF"
    dword = 0;
    fwrite(&dword, 1, 4, foutput); // remaining file size
    fwrite("WAVE", 4, 1, foutput); // "WAVE"

    fwrite("fmt ", 4, 1, foutput); // "fmt "
    dword = 16;
    fwrite(&dword, 1, 4, foutput); // chunk size
    word = 1;
    fwrite(&word, 1, 2, foutput); // compression code
    word = 1;
    fwrite(&word, 1, 2, foutput); // channels
    dword = wav.wav_freq;
    fwrite(&dword, 1, 4, foutput); // sample rate
    dword = wav.wav_freq * wav.wav_bits / 8;
    fwrite(&dword, 1, 4, foutput); // bytes/sec
    word = wav.wav_bits / 8;
    fwrite(&word, 1, 2, foutput); // block align
    word = wav.wav_bits;
    fwrite(&word, 1, 2, foutput); // bits per sample

    fwrite("data", 4, 1, foutput); // "data"
    dword = 0;
    int foutstream_datasize = ftell(foutput);
    fwrite(&dword, 1, 4, foutput); // chunk size

    // write sample data
    mute_stream = true;
    wav.file = foutput;
    wav.file_sampleswritten = 0;
    wav.filesample = 0.0f;
    wav.fileacc = 0;
    play();
    while (playing_sample) {
        synthSample(256, &wav);
    }
    mute_stream = false;

    // seek back to header and write size info
    fseek(foutput, 4, SEEK_SET);
    dword = 0;
    dword = foutstream_datasize - 4 + wav.file_sampleswritten * wav.wav_bits / 8;
    fwrite(&dword, 1, 4, foutput); // remaining file size
    fseek(foutput, foutstream_datasize, SEEK_SET);
    dword = wav.file_sampleswritten * wav.wav_bits / 8;
    fwrite(&dword, 1, 4, foutput); // chunk size (data)
    fclose(foutput);

    return true;
}

void Synthesizer::sdlAudioCallback(void* userdata, Uint8* stream, int len) {
    Synthesizer* synth = reinterpret_cast<Synthesizer*>(userdata);

    if (synth->playing_sample && !synth->mute_stream) {
        unsigned int l = len / 2;
        float fbuf[l];
        memset(fbuf, 0, sizeof(fbuf));
        BufferStrategy strategy(fbuf);
        synth->synthSample(l, &strategy);
        while (l--) {
            float f = fbuf[l];
            if (f < -1.0) {
                f = -1.0;
            }
            if (f > 1.0) {
                f = 1.0;
            }
            ((Sint16*)stream)[l] = (Sint16)(f * 32767);
        }
    } else {
        memset(stream, 0, len);
    }
}

void Synthesizer::init() {
    SDL_AudioSpec des;
    des.freq = 44100;
    des.format = AUDIO_S16SYS;
    des.channels = 1;
    des.samples = 512;
    des.callback = sdlAudioCallback;
    des.userdata = this;
    if (SDL_OpenAudio(&des, NULL) != 0) {
        fprintf(stderr, "Failed to init audio\n");
        exit(1);
    }
    SDL_PauseAudio(0);
}

void Synthesizer::schedulePlay() {
    // FIXME: Hack
    if (!mInited) {
        init();
        mInited = true;
    }
    mPlayTimer->start();
}
