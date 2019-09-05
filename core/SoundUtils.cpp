#include <SoundUtils.h>

#include <QMetaProperty>

#include <math.h>

#include <Sound.h>

namespace SoundUtils {

inline int rnd(int n) {
    return rand() % (n + 1);
}

inline float frnd(float range) {
    return (float)rnd(10000) / 10000 * range;
}

std::unique_ptr<Sound> generatePickup() {
    auto sound = std::make_unique<Sound>();
    sound->setBaseFrequency(0.4f + frnd(0.5f));
    sound->setAttackTime(0.0f);
    sound->setSustainTime(frnd(0.1f));
    sound->setDecayTime(0.1f + frnd(0.4f));
    sound->setSustainPunch(0.3f + frnd(0.3f));
    if (rnd(1)) {
        sound->setChangeSpeed(0.5f + frnd(0.2f));
        sound->setChangeAmount(0.2f + frnd(0.4f));
    }
    return sound;
}

std::unique_ptr<Sound> generateLaser() {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(WaveForm::random({WaveForm::Square,
                                         WaveForm::Sawtooth,
                                         WaveForm::Sine}));
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
    return sound;
}

std::unique_ptr<Sound> generateExplosion() {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(WaveForm::Noise);
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
    return sound;
}

std::unique_ptr<Sound> generatePowerup() {
    auto sound = std::make_unique<Sound>();
    if (rnd(1)) {
        sound->setWaveForm(WaveForm::Sawtooth);
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
    return sound;
}

std::unique_ptr<Sound> generateHitHurt() {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(WaveForm::random({WaveForm::Square,
                                         WaveForm::Sawtooth,
                                         WaveForm::Noise}));
    if (sound->waveForm() == WaveForm::Square) {
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
    return sound;
}

std::unique_ptr<Sound> generateJump() {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(WaveForm::Square);
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
    return sound;
}

std::unique_ptr<Sound> generateBlipSelect() {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(WaveForm::random({WaveForm::Square, WaveForm::Sawtooth}));
    if (sound->waveForm() == WaveForm::Square) {
        sound->setSquareDuty(frnd(0.6f));
    }
    sound->setBaseFrequency(0.2f + frnd(0.4f));
    sound->setAttackTime(0.0f);
    sound->setSustainTime(0.1f + frnd(0.1f));
    sound->setDecayTime(frnd(0.2f));
    sound->setHpFilterCutoff(0.1f);
    return sound;
}

void mutate(Sound* sound) {
    QMetaObject mo = BaseSound::staticMetaObject;
    for (int i = 0; i < mo.propertyCount(); ++i) {
        QMetaProperty property = mo.property(i);
        if (property.type() == QVariant::Double) {
            qreal value = property.read(sound).toReal() + frnd(0.1f) - 0.05f;
            property.write(sound, value);
        }
    }
}

} // namespace
