#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QMutex>
#include <QObject>
#include <QVector>

class QTimer;

class Sound;

class SoundPlayer : public QObject {
    Q_OBJECT
    Q_PROPERTY(Sound* sound READ sound WRITE setSound NOTIFY soundChanged)
    Q_PROPERTY(bool loop READ loop WRITE setLoop NOTIFY loopChanged)
public:
    explicit SoundPlayer(QObject* parent = nullptr);
    ~SoundPlayer();

    Q_INVOKABLE void play();

    Sound* sound() const;
    void setSound(Sound* value);

    bool loop() const;
    void setLoop(bool value);

signals:
    void soundChanged(Sound* value);
    void loopChanged(bool value);

private:
    bool mPlaying = false;
    bool mLoop = false;
    QTimer* mPlayTimer;
    Sound* mSound = nullptr;

    mutable QMutex mMutex;
    struct PlayThreadData {
        QVector<qreal> samples;
        bool playing = false;
        bool loop = false;
        int position = 0;
    } mPlayThreadData;

    void sdlAudioCallback(unsigned char* stream, int len);
    void registerCallback();
    void unregisterCallback();

    void startPlaying();

    static void staticSdlAudioCallback(void* userdata, unsigned char* stream, int len);

    void onSoundModified();
    void updateSamples();
};

#endif // SOUNDPLAYER_H
