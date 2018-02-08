#include <generator.h>

#include <QMetaProperty>
#include <QQmlEngine>

#include <math.h>

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

void Generator::generatePickup() {
    Sound* sound = createSound();
    sound->setBaseFrequency(0.4f + frnd(0.5f));
    sound->setAttackTime(0.0f);
    sound->setSustainTime(frnd(0.1f));
    sound->setDecayTime(0.1f + frnd(0.4f));
    sound->setSustainPunch(0.3f + frnd(0.3f));
    if (rnd(1)) {
        sound->setChangeSpeed(0.5f + frnd(0.2f));
        sound->setChangeAmount(0.2f + frnd(0.4f));
    }
    soundGenerated(tr("Pickup"), sound);
}

void Generator::generateLaser() {
    Sound* sound = createSound();
    int wave_type = rnd(2);
    if (wave_type == 2 && rnd(1)) {
        wave_type = rnd(1);
    }
    sound->setWaveType(wave_type);
    sound->setBaseFrequency(0.5f + frnd(0.5f));
    sound->setMinFrequency(sound->baseFrequency() - 0.2f - frnd(0.6f));
    if (sound->minFrequency() < 0.2f) {
        sound->setMinFrequency(0.2f);
    }
    sound->setSlide(-0.15f - frnd(0.2f));
    if (rnd(2) == 0) {
        sound->setBaseFrequency(0.3f + frnd(0.6f));
        sound->setMinFrequency(frnd(0.1f));
        sound->setSlide(-0.35f - frnd(0.3f));
    }
    if (rnd(1)) {
        sound->setSquareDuty(frnd(0.5f));
        sound->setDutySweep(frnd(0.2f));
    } else {
        sound->setSquareDuty(0.4f + frnd(0.5f));
        sound->setDutySweep(-frnd(0.7f));
    }
    sound->setAttackTime(0.0f);
    sound->setSustainTime(0.1f + frnd(0.2f));
    sound->setDecayTime(frnd(0.4f));
    if (rnd(1)) {
        sound->setSustainPunch(frnd(0.3f));
    }
    if (rnd(2) == 0) {
        sound->setPhaserOffset(frnd(0.2f));
        sound->setPhaserSweep(-frnd(0.2f));
    }
    if (rnd(1)) {
        sound->setHpFilterCutoff(frnd(0.3f));
    }
    soundGenerated(tr("Laser"), sound);
}

void Generator::generateExplosion() {
    Sound* sound = createSound();
    sound->setWaveType(3);
    if (rnd(1)) {
        sound->setBaseFrequency(0.1f + frnd(0.4f));
        sound->setSlide(-0.1f + frnd(0.4f));
    } else {
        sound->setBaseFrequency(0.2f + frnd(0.7f));
        sound->setSlide(-0.2f - frnd(0.2f));
    }
    sound->setBaseFrequency(sound->baseFrequency() * sound->baseFrequency());
    if (rnd(4) == 0) {
        sound->setSlide(0.0f);
    }
    if (rnd(2) == 0) {
        sound->setRepeatSpeed(0.3f + frnd(0.5f));
    }
    sound->setAttackTime(0.0f);
    sound->setSustainTime(0.1f + frnd(0.3f));
    sound->setDecayTime(frnd(0.5f));
    if (rnd(1) == 0) {
        sound->setPhaserOffset(-0.3f + frnd(0.9f));
        sound->setPhaserSweep(-frnd(0.3f));
    }
    sound->setSustainPunch(0.2f + frnd(0.6f));
    if (rnd(1)) {
        sound->setVibratoDepth(frnd(0.7f));
        sound->setVibratoSpeed(frnd(0.6f));
    }
    if (rnd(2) == 0) {
        sound->setChangeSpeed(0.6f + frnd(0.3f));
        sound->setChangeAmount(0.8f - frnd(1.6f));
    }
    soundGenerated(tr("Explosion"), sound);
}

void Generator::generatePowerup() {
    Sound* sound = createSound();
    if (rnd(1)) {
        sound->setWaveType(1);
    } else {
        sound->setSquareDuty(frnd(0.6f));
    }
    if (rnd(1)) {
        sound->setBaseFrequency(0.2f + frnd(0.3f));
        sound->setSlide(0.1f + frnd(0.4f));
        sound->setRepeatSpeed(0.4f + frnd(0.4f));
    } else {
        sound->setBaseFrequency(0.2f + frnd(0.3f));
        sound->setSlide(0.05f + frnd(0.2f));
        if (rnd(1)) {
            sound->setVibratoDepth(frnd(0.7f));
            sound->setVibratoSpeed(frnd(0.6f));
        }
    }
    sound->setAttackTime(0.0f);
    sound->setSustainTime(frnd(0.4f));
    sound->setDecayTime(0.1f + frnd(0.4f));
    soundGenerated(tr("Power up"), sound);
}

void Generator::generateHitHurt() {
    Sound* sound = createSound();
    sound->setWaveType(rnd(2));
    if (sound->waveType() == 2) {
        sound->setWaveType(3);
    }
    if (sound->waveType() == 0) {
        sound->setSquareDuty(frnd(0.6f));
    }
    sound->setBaseFrequency(0.2f + frnd(0.6f));
    sound->setSlide(-0.3f - frnd(0.4f));
    sound->setAttackTime(0.0f);
    sound->setSustainTime(frnd(0.1f));
    sound->setDecayTime(0.1f + frnd(0.2f));
    if (rnd(1)) {
        sound->setHpFilterCutoff(frnd(0.3f));
    }
    soundGenerated(tr("Hit"), sound);
}

void Generator::generateJump() {
    Sound* sound = createSound();
    sound->setWaveType(0);
    sound->setSquareDuty(frnd(0.6f));
    sound->setBaseFrequency(0.3f + frnd(0.3f));
    sound->setSlide(0.1f + frnd(0.2f));
    sound->setAttackTime(0.0f);
    sound->setSustainTime(0.1f + frnd(0.3f));
    sound->setDecayTime(0.1f + frnd(0.2f));
    if (rnd(1)) {
        sound->setHpFilterCutoff(frnd(0.3f));
    }
    if (rnd(1)) {
        sound->setLpFilterCutoff(1.0f - frnd(0.6f));
    }
    soundGenerated(tr("Jump"), sound);
}

void Generator::generateBlipSelect() {
    Sound* sound = createSound();
    sound->setWaveType(rnd(1));
    if (sound->waveType() == 0) {
        sound->setSquareDuty(frnd(0.6f));
    }
    sound->setBaseFrequency(0.2f + frnd(0.4f));
    sound->setAttackTime(0.0f);
    sound->setSustainTime(0.1f + frnd(0.1f));
    sound->setDecayTime(frnd(0.2f));
    sound->setHpFilterCutoff(0.1f);
    soundGenerated(tr("Blip"), sound);
}

void Generator::mutate(Sound* source) {
    Sound* sound = createSound();
    sound->fromOther(source);
    QMetaObject mo = BaseSound::staticMetaObject;
    for (int i = 0; i < mo.propertyCount(); ++i) {
        QMetaProperty property = mo.property(i);
        if (property.type() == QVariant::Double) {
            qreal value = property.read(sound).toReal() + frnd(0.1f) - 0.05f;
            property.write(sound, value);
        }
    }
    soundGenerated(tr("Mutated"), sound);
}

Sound* Generator::createSound() {
    Sound* sound = new Sound;
    QQmlEngine::setObjectOwnership(sound, QQmlEngine::CppOwnership);
    return sound;
}
