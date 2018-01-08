#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>

#include "synthesizer.h"

class Generator : public QObject {
    Q_OBJECT
    Q_PROPERTY(Synthesizer* synth READ synth WRITE setSynth NOTIFY synthChanged)
public:
    explicit Generator(QObject* parent = nullptr);

    Synthesizer* synth() const;
    void setSynth(Synthesizer* value);

    Q_INVOKABLE void generatePickup();
    Q_INVOKABLE void generateLaser();
    Q_INVOKABLE void generateExplosion();
    Q_INVOKABLE void generatePowerup();
    Q_INVOKABLE void generateHitHurt();
    Q_INVOKABLE void generateJump();
    Q_INVOKABLE void generateBlipSelect();

signals:
    void synthChanged(Synthesizer* value);

private:
    Synthesizer* mSynth;
};

#endif // GENERATOR_H
