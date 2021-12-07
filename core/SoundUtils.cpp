#include "SoundUtils.h"

#include <QMetaProperty>
#include <cmath>

#include "Sound.h"

using std::pow;

namespace SoundUtils {

inline int rnd(int n) {
    return std::rand() % (n + 1);
}

inline qreal frnd(qreal range) {
    return qreal(rnd(10000)) / 10000.0 * range;
}

std::unique_ptr<Sound> generatePickup() {
    auto sound = std::make_unique<Sound>();
    sound->setBaseFrequency(0.4 + frnd(0.5));
    sound->setAttackTime(0.0);
    sound->setSustainTime(frnd(0.1));
    sound->setDecayTime(0.1 + frnd(0.4));
    sound->setSustainPunch(0.3 + frnd(0.3));
    if (rnd(1)) {
        sound->setChangeSpeed(0.5 + frnd(0.2));
        sound->setChangeAmount(0.2 + frnd(0.4));
    }
    return sound;
}

std::unique_ptr<Sound> generateLaser() {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(WaveForm::random({WaveForm::Square, WaveForm::Sawtooth, WaveForm::Sine}));
    sound->setBaseFrequency(0.5 + frnd(0.5));
    sound->setMinFrequency(sound->baseFrequency() - 0.2 - frnd(0.6));
    if (sound->minFrequency() < 0.2) {
        sound->setMinFrequency(0.2);
    }
    sound->setSlide(-0.15 - frnd(0.2));
    if (rnd(2) == 0) {
        sound->setBaseFrequency(0.3 + frnd(0.6));
        sound->setMinFrequency(frnd(0.1));
        sound->setSlide(-0.35 - frnd(0.3));
    }
    if (rnd(1)) {
        sound->setSquareDuty(frnd(0.5));
        sound->setDutySweep(frnd(0.2));
    } else {
        sound->setSquareDuty(0.4 + frnd(0.5));
        sound->setDutySweep(-frnd(0.7));
    }
    sound->setAttackTime(0.0);
    sound->setSustainTime(0.1 + frnd(0.2));
    sound->setDecayTime(frnd(0.4));
    if (rnd(1)) {
        sound->setSustainPunch(frnd(0.3));
    }
    if (rnd(2) == 0) {
        sound->setPhaserOffset(frnd(0.2));
        sound->setPhaserSweep(-frnd(0.2));
    }
    if (rnd(1)) {
        sound->setHpFilterCutoff(frnd(0.3));
    }
    return sound;
}

std::unique_ptr<Sound> generateExplosion() {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(WaveForm::Noise);
    if (rnd(1)) {
        sound->setBaseFrequency(0.1 + frnd(0.4));
        sound->setSlide(-0.1 + frnd(0.4));
    } else {
        sound->setBaseFrequency(0.2 + frnd(0.7));
        sound->setSlide(-0.2 - frnd(0.2));
    }
    sound->setBaseFrequency(sound->baseFrequency() * sound->baseFrequency());
    if (rnd(4) == 0) {
        sound->setSlide(0.0);
    }
    if (rnd(2) == 0) {
        sound->setRepeatSpeed(0.3 + frnd(0.5));
    }
    sound->setAttackTime(0.0);
    sound->setSustainTime(0.1 + frnd(0.3));
    sound->setDecayTime(frnd(0.5));
    if (rnd(1) == 0) {
        sound->setPhaserOffset(-0.3 + frnd(0.9));
        sound->setPhaserSweep(-frnd(0.3));
    }
    sound->setSustainPunch(0.2 + frnd(0.6));
    if (rnd(1)) {
        sound->setVibratoDepth(frnd(0.7));
        sound->setVibratoSpeed(frnd(0.6));
    }
    if (rnd(2) == 0) {
        sound->setChangeSpeed(0.6 + frnd(0.3));
        sound->setChangeAmount(0.8 - frnd(1.6));
    }
    return sound;
}

std::unique_ptr<Sound> generatePowerup() {
    auto sound = std::make_unique<Sound>();
    if (rnd(1)) {
        sound->setWaveForm(WaveForm::Sawtooth);
    } else {
        sound->setSquareDuty(frnd(0.6));
    }
    if (rnd(1)) {
        sound->setBaseFrequency(0.2 + frnd(0.3));
        sound->setSlide(0.1 + frnd(0.4));
        sound->setRepeatSpeed(0.4 + frnd(0.4));
    } else {
        sound->setBaseFrequency(0.2 + frnd(0.3));
        sound->setSlide(0.05 + frnd(0.2));
        if (rnd(1)) {
            sound->setVibratoDepth(frnd(0.7));
            sound->setVibratoSpeed(frnd(0.6));
        }
    }
    sound->setAttackTime(0.0);
    sound->setSustainTime(frnd(0.4));
    sound->setDecayTime(0.1 + frnd(0.4));
    return sound;
}

std::unique_ptr<Sound> generateHitHurt() {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(WaveForm::random({WaveForm::Square, WaveForm::Sawtooth, WaveForm::Noise}));
    if (sound->waveForm() == WaveForm::Square) {
        sound->setSquareDuty(frnd(0.6));
    }
    sound->setBaseFrequency(0.2 + frnd(0.6));
    sound->setSlide(-0.3 - frnd(0.4));
    sound->setAttackTime(0.0);
    sound->setSustainTime(frnd(0.1));
    sound->setDecayTime(0.1 + frnd(0.2));
    if (rnd(1)) {
        sound->setHpFilterCutoff(frnd(0.3));
    }
    return sound;
}

std::unique_ptr<Sound> generateJump() {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(WaveForm::Square);
    sound->setSquareDuty(frnd(0.6));
    sound->setBaseFrequency(0.3 + frnd(0.3));
    sound->setSlide(0.1 + frnd(0.2));
    sound->setAttackTime(0.0);
    sound->setSustainTime(0.1 + frnd(0.3));
    sound->setDecayTime(0.1 + frnd(0.2));
    if (rnd(1)) {
        sound->setHpFilterCutoff(frnd(0.3));
    }
    if (rnd(1)) {
        sound->setLpFilterCutoff(1.0 - frnd(0.6));
    }
    return sound;
}

std::unique_ptr<Sound> generateBlipSelect() {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(WaveForm::random({WaveForm::Square, WaveForm::Sawtooth}));
    if (sound->waveForm() == WaveForm::Square) {
        sound->setSquareDuty(frnd(0.6));
    }
    sound->setBaseFrequency(0.2 + frnd(0.4));
    sound->setAttackTime(0.0);
    sound->setSustainTime(0.1 + frnd(0.1));
    sound->setDecayTime(frnd(0.2));
    sound->setHpFilterCutoff(0.1);
    return sound;
}

std::unique_ptr<Sound> randomize(WaveForm::Enum waveForm) {
    auto sound = std::make_unique<Sound>();
    sound->setWaveForm(waveForm);

    if (rnd(1)) {
        sound->setBaseFrequency(pow(frnd(2.0) - 1.0, 3.0) + 0.5);
    } else {
        sound->setBaseFrequency(pow(frnd(2.0) - 1.0, 2.0));
    }
    sound->setMinFrequency(0);

    qreal p_freq_ramp = pow(frnd(2.0) - 1.0, 5.0);
    if (sound->baseFrequency() > 0.7 && p_freq_ramp > 0.2)
        p_freq_ramp = -p_freq_ramp;
    if (sound->baseFrequency() < 0.2 && p_freq_ramp < -0.05)
        p_freq_ramp = -p_freq_ramp;
    sound->setSlide(p_freq_ramp);

    sound->setDeltaSlide(pow(frnd(2.0) - 1.0, 3.0));

    sound->setSquareDuty(frnd(2.0) - 1.0);
    sound->setDutySweep(pow(frnd(2.0) - 1.0, 3.0));

    sound->setVibratoDepth(pow(frnd(2.0) - 1.0, 3.0));
    sound->setVibratoSpeed(frnd(2.0) - 1.0);

    sound->setAttackTime(pow(frnd(2.0) - 1.0, 3.0));
    sound->setSustainTime(pow(frnd(2.0) - 1.0, 2.0));
    sound->setDecayTime(frnd(2.0) - 1.0);
    sound->setSustainPunch(pow(frnd(0.8), 2.0));

    if (sound->attackTime() + sound->sustainTime() + sound->decayTime() < 0.2) {
        sound->setSustainTime(sound->sustainTime() + 0.2 + frnd(0.3));
        sound->setDecayTime(sound->decayTime() + 0.2 + frnd(0.3));
    }

    sound->setLpFilterResonance(frnd(2.0) - 1.0);
    sound->setLpFilterCutoff(1.0 - pow(frnd(1.0), 3.0));
    sound->setLpFilterCutoffSweep(pow(frnd(2.0) - 1.0, 3.0));

    if (sound->lpFilterResonance() < 0.1 && sound->lpFilterCutoffSweep() < -0.05) {
        sound->setLpFilterCutoffSweep(-sound->lpFilterCutoffSweep());
    }

    sound->setHpFilterCutoff(pow(frnd(1.0), 5.0));
    sound->setHpFilterCutoffSweep(pow(frnd(2.0) - 1.0, 5.0));

    sound->setPhaserOffset(pow(frnd(2.0) - 1.0, 3.0));
    sound->setPhaserSweep(pow(frnd(2.0) - 1.0, 3.0));

    sound->setRepeatSpeed(frnd(2.0) - 1.0);

    sound->setChangeSpeed(frnd(2.0) - 1.0);
    sound->setChangeAmount(frnd(2.0) - 1.0);

    return sound;
}

void mutate(Sound* sound) {
    QMetaObject mo = BaseSound::staticMetaObject;
    for (int i = 0; i < mo.propertyCount(); ++i) {
        QMetaProperty property = mo.property(i);
        if (property.type() == QVariant::Double) {
            qreal value = property.read(sound).toReal() + frnd(0.1) - 0.05;
            property.write(sound, value);
        }
    }
}

} // namespace SoundUtils
