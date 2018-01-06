#include "synthesizer.h"

#include "audio.h"

Synthesizer::Synthesizer(QObject *parent) : QObject(parent)
{

}

void Synthesizer::generatePickup()
{
    PickupCoin();
    PlaySample();
}

void Synthesizer::generateLaser()
{
    LaserShoot();
    PlaySample();
}

void Synthesizer::generateExplosion()
{
    Explosion();
    PlaySample();
}

void Synthesizer::generatePowerup()
{
    Powerup();
    PlaySample();
}

void Synthesizer::generateHitHurt()
{
    HitHurt();
    PlaySample();
}

void Synthesizer::generateJump()
{
    Jump();
    PlaySample();
}

void Synthesizer::generateBlipSelect()
{
    BlipSelect();
    PlaySample();
}
