#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QMutex>
#include <QObject>

#include <memory>

class QTimer;

class Sound;
class Synthesizer;

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
        bool playing = false;
        bool loop = false;
        std::unique_ptr<Synthesizer> synth;
    } mPlayThreadData;

    void sdlAudioCallback(unsigned char* stream, int len);
    void registerCallback();
    void unregisterCallback();
    void schedulePlay();

    void onSoundModified();

    static void staticSdlAudioCallback(void* userdata, unsigned char* stream, int len);
};

#endif // SOUNDPLAYER_H
