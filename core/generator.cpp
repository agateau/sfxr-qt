#include <generator.h>

inline int rnd(int n) {
    return rand() % (n + 1);
}

inline float frnd(float range) {
    return (float)rnd(10000) / 10000 * range;
}

Generator::Generator(QObject* parent)
    : QObject(parent) {
}

Synthesizer* Generator::synth() const {
    return mSynth;
}

void Generator::setSynth(Synthesizer* value) {
    if (mSynth == value) {
        return;
    }
    mSynth = value;
    synthChanged(value);
}

void Generator::generatePickup() {
    mSynth->resetParams();
    mSynth->setBaseFrequency(0.4f + frnd(0.5f));
    mSynth->setAttackTime(0.0f);
    mSynth->setSustainTime(frnd(0.1f));
    mSynth->setDecayTime(0.1f + frnd(0.4f));
    mSynth->setSustainPunch(0.3f + frnd(0.3f));
    if (rnd(1)) {
        mSynth->setChangeSpeed(0.5f + frnd(0.2f));
        mSynth->setChangeAmount(0.2f + frnd(0.4f));
    }
}

void Generator::generateLaser() {
    mSynth->resetParams();
    int wave_type = rnd(2);
    if (wave_type == 2 && rnd(1)) {
        wave_type = rnd(1);
    }
    mSynth->setWaveType(wave_type);
    mSynth->setBaseFrequency(0.5f + frnd(0.5f));
    mSynth->setMinFrequency(mSynth->baseFrequency() - 0.2f - frnd(0.6f));
    if (mSynth->minFrequency() < 0.2f) {
        mSynth->setMinFrequency(0.2f);
    }
    mSynth->setSlide(-0.15f - frnd(0.2f));
    if (rnd(2) == 0) {
        mSynth->setBaseFrequency(0.3f + frnd(0.6f));
        mSynth->setMinFrequency(frnd(0.1f));
        mSynth->setSlide(-0.35f - frnd(0.3f));
    }
    if (rnd(1)) {
        mSynth->setSquareDuty(frnd(0.5f));
        mSynth->setDutySweep(frnd(0.2f));
    } else {
        mSynth->setSquareDuty(0.4f + frnd(0.5f));
        mSynth->setDutySweep(-frnd(0.7f));
    }
    mSynth->setAttackTime(0.0f);
    mSynth->setSustainTime(0.1f + frnd(0.2f));
    mSynth->setDecayTime(frnd(0.4f));
    if (rnd(1)) {
        mSynth->setSustainPunch(frnd(0.3f));
    }
    if (rnd(2) == 0) {
        mSynth->setPhaserOffset(frnd(0.2f));
        mSynth->setPhaserSweep(-frnd(0.2f));
    }
    if (rnd(1)) {
        mSynth->setHpFilterCutoff(frnd(0.3f));
    }
}

void Generator::generateExplosion() {
    mSynth->resetParams();
    mSynth->setWaveType(3);
    if (rnd(1)) {
        mSynth->setBaseFrequency(0.1f + frnd(0.4f));
        mSynth->setSlide(-0.1f + frnd(0.4f));
    } else {
        mSynth->setBaseFrequency(0.2f + frnd(0.7f));
        mSynth->setSlide(-0.2f - frnd(0.2f));
    }
    mSynth->setBaseFrequency(mSynth->baseFrequency() * mSynth->baseFrequency());
    if (rnd(4) == 0) {
        mSynth->setSlide(0.0f);
    }
    if (rnd(2) == 0) {
        mSynth->setRepeatSpeed(0.3f + frnd(0.5f));
    }
    mSynth->setAttackTime(0.0f);
    mSynth->setSustainTime(0.1f + frnd(0.3f));
    mSynth->setDecayTime(frnd(0.5f));
    if (rnd(1) == 0) {
        mSynth->setPhaserOffset(-0.3f + frnd(0.9f));
        mSynth->setPhaserSweep(-frnd(0.3f));
    }
    mSynth->setSustainPunch(0.2f + frnd(0.6f));
    if (rnd(1)) {
        mSynth->setVibratoDepth(frnd(0.7f));
        mSynth->setVibratoSpeed(frnd(0.6f));
    }
    if (rnd(2) == 0) {
        mSynth->setChangeSpeed(0.6f + frnd(0.3f));
        mSynth->setChangeAmount(0.8f - frnd(1.6f));
    }
}

void Generator::generatePowerup() {
    mSynth->resetParams();
    if (rnd(1)) {
        mSynth->setWaveType(1);
    } else {
        mSynth->setSquareDuty(frnd(0.6f));
    }
    if (rnd(1)) {
        mSynth->setBaseFrequency(0.2f + frnd(0.3f));
        mSynth->setSlide(0.1f + frnd(0.4f));
        mSynth->setRepeatSpeed(0.4f + frnd(0.4f));
    } else {
        mSynth->setBaseFrequency(0.2f + frnd(0.3f));
        mSynth->setSlide(0.05f + frnd(0.2f));
        if (rnd(1)) {
            mSynth->setVibratoDepth(frnd(0.7f));
            mSynth->setVibratoSpeed(frnd(0.6f));
        }
    }
    mSynth->setAttackTime(0.0f);
    mSynth->setSustainTime(frnd(0.4f));
    mSynth->setDecayTime(0.1f + frnd(0.4f));
}

void Generator::generateHitHurt() {
    mSynth->resetParams();
    mSynth->setWaveType(rnd(2));
    if (mSynth->waveType() == 2) {
        mSynth->setWaveType(3);
    }
    if (mSynth->waveType() == 0) {
        mSynth->setSquareDuty(frnd(0.6f));
    }
    mSynth->setBaseFrequency(0.2f + frnd(0.6f));
    mSynth->setSlide(-0.3f - frnd(0.4f));
    mSynth->setAttackTime(0.0f);
    mSynth->setSustainTime(frnd(0.1f));
    mSynth->setDecayTime(0.1f + frnd(0.2f));
    if (rnd(1)) {
        mSynth->setHpFilterCutoff(frnd(0.3f));
    }
}

void Generator::generateJump() {
    mSynth->resetParams();
    mSynth->setWaveType(0);
    mSynth->setSquareDuty(frnd(0.6f));
    mSynth->setBaseFrequency(0.3f + frnd(0.3f));
    mSynth->setSlide(0.1f + frnd(0.2f));
    mSynth->setAttackTime(0.0f);
    mSynth->setSustainTime(0.1f + frnd(0.3f));
    mSynth->setDecayTime(0.1f + frnd(0.2f));
    if (rnd(1)) {
        mSynth->setHpFilterCutoff(frnd(0.3f));
    }
    if (rnd(1)) {
        mSynth->setLpFilterCutoff(1.0f - frnd(0.6f));
    }
}

void Generator::generateBlipSelect() {
    mSynth->resetParams();
    mSynth->setWaveType(rnd(1));
    if (mSynth->waveType() == 0) {
        mSynth->setSquareDuty(frnd(0.6f));
    }
    mSynth->setBaseFrequency(0.2f + frnd(0.4f));
    mSynth->setAttackTime(0.0f);
    mSynth->setSustainTime(0.1f + frnd(0.1f));
    mSynth->setDecayTime(frnd(0.2f));
    mSynth->setHpFilterCutoff(0.1f);
}
