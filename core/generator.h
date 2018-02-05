#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>

class Sound;

class Generator : public QObject {
    Q_OBJECT
    Q_PROPERTY(Sound* sound READ sound WRITE setSound NOTIFY soundChanged)
public:
    explicit Generator(QObject* parent = nullptr);

    Sound* sound() const;
    void setSound(Sound* value);

    Q_INVOKABLE void generatePickup();
    Q_INVOKABLE void generateLaser();
    Q_INVOKABLE void generateExplosion();
    Q_INVOKABLE void generatePowerup();
    Q_INVOKABLE void generateHitHurt();
    Q_INVOKABLE void generateJump();
    Q_INVOKABLE void generateBlipSelect();
    Q_INVOKABLE void mutate();
    Q_INVOKABLE void randomize();

signals:
    void soundChanged(Sound* value);
    void soundGenerated(Sound* value);

private:
    Sound* mSound = nullptr;
};

#endif // GENERATOR_H
