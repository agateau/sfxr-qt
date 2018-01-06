#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <QObject>

class Synthesizer : public QObject
{
    Q_OBJECT
public:
    explicit Synthesizer(QObject *parent = nullptr);

    Q_INVOKABLE void generatePickup();
    Q_INVOKABLE void generateLaser();
    Q_INVOKABLE void generateExplosion();
    Q_INVOKABLE void generatePowerup();
    Q_INVOKABLE void generateHitHurt();
    Q_INVOKABLE void generateJump();
    Q_INVOKABLE void generateBlipSelect();
};

#endif // SYNTHESIZER_H
