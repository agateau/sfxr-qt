#include <generator.h>

#include <QMetaProperty>

#include <sound.h>

inline int rnd(int n) {
    return rand() % (n + 1);
}

inline float frnd(float range) {
    return (float)rnd(10000) / 10000 * range;
}

Generator::Generator(QObject* parent)
    : QObject(parent) {
}

Sound* Generator::sound() const {
    return mSound;
}

void Generator::setSound(Sound* value) {
    if (mSound == value) {
        return;
    }
    mSound = value;
    soundChanged(value);
}

void Generator::generatePickup() {
    mSound->resetParams();
    mSound->setBaseFrequency(0.4f + frnd(0.5f));
    mSound->setAttackTime(0.0f);
    mSound->setSustainTime(frnd(0.1f));
    mSound->setDecayTime(0.1f + frnd(0.4f));
    mSound->setSustainPunch(0.3f + frnd(0.3f));
    if (rnd(1)) {
        mSound->setChangeSpeed(0.5f + frnd(0.2f));
        mSound->setChangeAmount(0.2f + frnd(0.4f));
    }
}

void Generator::generateLaser() {
    mSound->resetParams();
    int wave_type = rnd(2);
    if (wave_type == 2 && rnd(1)) {
        wave_type = rnd(1);
    }
    mSound->setWaveType(wave_type);
    mSound->setBaseFrequency(0.5f + frnd(0.5f));
    mSound->setMinFrequency(mSound->baseFrequency() - 0.2f - frnd(0.6f));
    if (mSound->minFrequency() < 0.2f) {
        mSound->setMinFrequency(0.2f);
    }
    mSound->setSlide(-0.15f - frnd(0.2f));
    if (rnd(2) == 0) {
        mSound->setBaseFrequency(0.3f + frnd(0.6f));
        mSound->setMinFrequency(frnd(0.1f));
        mSound->setSlide(-0.35f - frnd(0.3f));
    }
    if (rnd(1)) {
        mSound->setSquareDuty(frnd(0.5f));
        mSound->setDutySweep(frnd(0.2f));
    } else {
        mSound->setSquareDuty(0.4f + frnd(0.5f));
        mSound->setDutySweep(-frnd(0.7f));
    }
    mSound->setAttackTime(0.0f);
    mSound->setSustainTime(0.1f + frnd(0.2f));
    mSound->setDecayTime(frnd(0.4f));
    if (rnd(1)) {
        mSound->setSustainPunch(frnd(0.3f));
    }
    if (rnd(2) == 0) {
        mSound->setPhaserOffset(frnd(0.2f));
        mSound->setPhaserSweep(-frnd(0.2f));
    }
    if (rnd(1)) {
        mSound->setHpFilterCutoff(frnd(0.3f));
    }
}

void Generator::generateExplosion() {
    mSound->resetParams();
    mSound->setWaveType(3);
    if (rnd(1)) {
        mSound->setBaseFrequency(0.1f + frnd(0.4f));
        mSound->setSlide(-0.1f + frnd(0.4f));
    } else {
        mSound->setBaseFrequency(0.2f + frnd(0.7f));
        mSound->setSlide(-0.2f - frnd(0.2f));
    }
    mSound->setBaseFrequency(mSound->baseFrequency() * mSound->baseFrequency());
    if (rnd(4) == 0) {
        mSound->setSlide(0.0f);
    }
    if (rnd(2) == 0) {
        mSound->setRepeatSpeed(0.3f + frnd(0.5f));
    }
    mSound->setAttackTime(0.0f);
    mSound->setSustainTime(0.1f + frnd(0.3f));
    mSound->setDecayTime(frnd(0.5f));
    if (rnd(1) == 0) {
        mSound->setPhaserOffset(-0.3f + frnd(0.9f));
        mSound->setPhaserSweep(-frnd(0.3f));
    }
    mSound->setSustainPunch(0.2f + frnd(0.6f));
    if (rnd(1)) {
        mSound->setVibratoDepth(frnd(0.7f));
        mSound->setVibratoSpeed(frnd(0.6f));
    }
    if (rnd(2) == 0) {
        mSound->setChangeSpeed(0.6f + frnd(0.3f));
        mSound->setChangeAmount(0.8f - frnd(1.6f));
    }
}

void Generator::generatePowerup() {
    mSound->resetParams();
    if (rnd(1)) {
        mSound->setWaveType(1);
    } else {
        mSound->setSquareDuty(frnd(0.6f));
    }
    if (rnd(1)) {
        mSound->setBaseFrequency(0.2f + frnd(0.3f));
        mSound->setSlide(0.1f + frnd(0.4f));
        mSound->setRepeatSpeed(0.4f + frnd(0.4f));
    } else {
        mSound->setBaseFrequency(0.2f + frnd(0.3f));
        mSound->setSlide(0.05f + frnd(0.2f));
        if (rnd(1)) {
            mSound->setVibratoDepth(frnd(0.7f));
            mSound->setVibratoSpeed(frnd(0.6f));
        }
    }
    mSound->setAttackTime(0.0f);
    mSound->setSustainTime(frnd(0.4f));
    mSound->setDecayTime(0.1f + frnd(0.4f));
}

void Generator::generateHitHurt() {
    mSound->resetParams();
    mSound->setWaveType(rnd(2));
    if (mSound->waveType() == 2) {
        mSound->setWaveType(3);
    }
    if (mSound->waveType() == 0) {
        mSound->setSquareDuty(frnd(0.6f));
    }
    mSound->setBaseFrequency(0.2f + frnd(0.6f));
    mSound->setSlide(-0.3f - frnd(0.4f));
    mSound->setAttackTime(0.0f);
    mSound->setSustainTime(frnd(0.1f));
    mSound->setDecayTime(0.1f + frnd(0.2f));
    if (rnd(1)) {
        mSound->setHpFilterCutoff(frnd(0.3f));
    }
}

void Generator::generateJump() {
    mSound->resetParams();
    mSound->setWaveType(0);
    mSound->setSquareDuty(frnd(0.6f));
    mSound->setBaseFrequency(0.3f + frnd(0.3f));
    mSound->setSlide(0.1f + frnd(0.2f));
    mSound->setAttackTime(0.0f);
    mSound->setSustainTime(0.1f + frnd(0.3f));
    mSound->setDecayTime(0.1f + frnd(0.2f));
    if (rnd(1)) {
        mSound->setHpFilterCutoff(frnd(0.3f));
    }
    if (rnd(1)) {
        mSound->setLpFilterCutoff(1.0f - frnd(0.6f));
    }
}

void Generator::generateBlipSelect() {
    mSound->resetParams();
    mSound->setWaveType(rnd(1));
    if (mSound->waveType() == 0) {
        mSound->setSquareDuty(frnd(0.6f));
    }
    mSound->setBaseFrequency(0.2f + frnd(0.4f));
    mSound->setAttackTime(0.0f);
    mSound->setSustainTime(0.1f + frnd(0.1f));
    mSound->setDecayTime(frnd(0.2f));
    mSound->setHpFilterCutoff(0.1f);
}

void Generator::mutate() {
    QMetaObject mo = BaseSound::staticMetaObject;
    for (int i = 0; i < mo.propertyCount(); ++i) {
        QMetaProperty property = mo.property(i);
        if (property.type() == QVariant::Double) {
            qreal value = property.read(mSound).toReal() + frnd(0.1f) - 0.05f;
            property.write(mSound, value);
        }
    }
}
