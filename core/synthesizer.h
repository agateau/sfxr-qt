#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <QObject>

#include <memory>

class Audio;

class Synthesizer : public QObject
{
    Q_OBJECT
public:
    explicit Synthesizer(QObject *parent = nullptr);
    ~Synthesizer();

    Q_INVOKABLE void generatePickup();
    Q_INVOKABLE void generateLaser();
    Q_INVOKABLE void generateExplosion();
    Q_INVOKABLE void generatePowerup();
    Q_INVOKABLE void generateHitHurt();
    Q_INVOKABLE void generateJump();
    Q_INVOKABLE void generateBlipSelect();

private:
    std::unique_ptr<Audio> mAudio;
};

#endif // SYNTHESIZER_H
