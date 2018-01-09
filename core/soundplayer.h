#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QObject>

#include <memory>

class QTimer;

class Sound;
class Synthesizer;

class SoundPlayer : public QObject {
    Q_OBJECT
    Q_PROPERTY(Sound* sound READ sound WRITE setSound NOTIFY soundChanged)
public:
    explicit SoundPlayer(QObject* parent = nullptr);
    ~SoundPlayer();

    Q_INVOKABLE void play();

    Sound* sound() const;
    void setSound(Sound* value);

signals:
    void soundChanged(Sound* value);

private:
    bool mPlaying = false;
    QTimer* mPlayTimer;
    Sound* mSound = nullptr;
    std::unique_ptr<Synthesizer> mSynth;

    void sdlAudioCallback(unsigned char* stream, int len);
    void init();
    void schedulePlay();

    static void staticSdlAudioCallback(void* userdata, unsigned char* stream, int len);
};

#endif // SOUNDPLAYER_H
