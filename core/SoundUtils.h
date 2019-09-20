#ifndef SOUNDUTILS_H
#define SOUNDUTILS_H

#include <Sound.h>

#include <memory>

/**
 * Functions to randomly generate or mutate sounds
 */
namespace SoundUtils {

std::unique_ptr<Sound> generatePickup();
std::unique_ptr<Sound> generateLaser();
std::unique_ptr<Sound> generateExplosion();
std::unique_ptr<Sound> generatePowerup();
std::unique_ptr<Sound> generateHitHurt();
std::unique_ptr<Sound> generateJump();
std::unique_ptr<Sound> generateBlipSelect();
std::unique_ptr<Sound> randomize(WaveForm::Enum waveForm);

void mutate(Sound* sound);

} // namespace SoundUtils

#endif /* SOUNDUTILS_H */
