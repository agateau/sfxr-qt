#include "Generator.h"

#include "Sound.h"
#include "SoundUtils.h"

using std::move;

Generator::Generator(QObject* parent) : QObject(parent) {
}

void Generator::generatePickup() {
    auto sound = SoundUtils::generatePickup();
    finish(move(sound), tr("Pickup"));
}

void Generator::generateLaser() {
    auto sound = SoundUtils::generateLaser();
    finish(move(sound), tr("Laser"));
}

void Generator::generateExplosion() {
    auto sound = SoundUtils::generateExplosion();
    finish(move(sound), tr("Explosion"));
}

void Generator::generatePowerup() {
    auto sound = SoundUtils::generatePowerup();
    finish(move(sound), tr("Power up"));
}

void Generator::generateHitHurt() {
    auto sound = SoundUtils::generateHitHurt();
    finish(move(sound), tr("Hit"));
}

void Generator::generateJump() {
    auto sound = SoundUtils::generateJump();
    finish(move(sound), tr("Jump"));
}

void Generator::generateBlipSelect() {
    auto sound = SoundUtils::generateBlipSelect();
    finish(move(sound), tr("Blip"));
}

void Generator::randomize(WaveForm::Enum waveForm) {
    auto sound = SoundUtils::randomize(waveForm);
    finish(move(sound), tr("Randomize"));
}

void Generator::mutate(Sound* source) {
    auto sound = std::make_unique<Sound>();
    sound->fromOther(source);
    SoundUtils::mutate(sound.get());
    finish(move(sound), tr("Mutated"));
}

void Generator::finish(std::unique_ptr<Sound> soundPtr, const QString& name) {
    auto sound = soundPtr.release();
    sound->setUnsavedName(name);
    soundGenerated(sound);
}
