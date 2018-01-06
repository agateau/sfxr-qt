#include "synthesizer.h"

#include "audio.h"

Synthesizer::Synthesizer(QObject *parent)
    : QObject(parent)
    , mAudio(new Audio)
{
    mAudio->Init();
}

Synthesizer::~Synthesizer()
{
}

void Synthesizer::generatePickup()
{
    mAudio->PickupCoin();
    mAudio->PlaySample();
}

void Synthesizer::generateLaser()
{
    mAudio->LaserShoot();
    mAudio->PlaySample();
}

void Synthesizer::generateExplosion()
{
    mAudio->Explosion();
    mAudio->PlaySample();
}

void Synthesizer::generatePowerup()
{
    mAudio->Powerup();
    mAudio->PlaySample();
}

void Synthesizer::generateHitHurt()
{
    mAudio->HitHurt();
    mAudio->PlaySample();
}

void Synthesizer::generateJump()
{
    mAudio->Jump();
    mAudio->PlaySample();
}

void Synthesizer::generateBlipSelect()
{
    mAudio->BlipSelect();
    mAudio->PlaySample();
}
