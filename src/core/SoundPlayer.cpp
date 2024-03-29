#include "SoundPlayer.h"

#include "Sound.h"
#include "Synthesizer.h"

#include <QDebug>
#include <QTimer>

#include <SDL.h>

class BufferStrategy : public Synthesizer::SynthStrategy {
public:
    BufferStrategy(QVector<qreal>* samples) : mSamples(samples) {
    }

    void write(qreal sample) override {
        auto value = qBound(-1., sample, 1.);
        mSamples->push_back(value);
    }

private:
    QVector<qreal>* const mSamples;
};

SoundPlayer::SoundPlayer(QObject* parent) : QObject(parent), mPlayTimer(new QTimer(this)) {
    mPlayThreadData.samples.reserve(65536);
    mPlayTimer->setInterval(0);
    mPlayTimer->setSingleShot(true);
    connect(mPlayTimer, &QTimer::timeout, this, &SoundPlayer::startPlaying);
    registerCallback();
}

SoundPlayer::~SoundPlayer() {
    unregisterCallback();
}

Sound* SoundPlayer::sound() const {
    return mSound;
}

void SoundPlayer::setSound(Sound* value) {
    if (mSound == value) {
        return;
    }
    if (mSound) {
        disconnect(mSound, nullptr, this, nullptr);
    }
    mSound = value;
    if (mSound) {
        updateSamples();
        connect(mSound, &Sound::modified, this, &SoundPlayer::onSoundModified);
    }
    {
        QMutexLocker lock(&mMutex);
        mPlayThreadData.playing = false;
        mPlayThreadData.position = 0;
    }
    soundChanged(value);
}

bool SoundPlayer::loop() const {
    return mLoop;
}

void SoundPlayer::setLoop(bool value) {
    if (mLoop == value) {
        return;
    }
    mLoop = value;
    {
        QMutexLocker lock(&mMutex);
        mPlayThreadData.loop = mLoop;
    }
    loopChanged(value);
}

QVector<qreal> SoundPlayer::samples() const {
    QMutexLocker lock(&mMutex);
    return mPlayThreadData.samples;
}

void SoundPlayer::startPlaying() {
    {
        QMutexLocker lock(&mMutex);
        mPlayThreadData.position = 0;
        mPlayThreadData.playing = true;
    }
    playPositionChanged(0);
}

void SoundPlayer::staticSdlAudioCallback(void* userdata, unsigned char* stream, int len) {
    SoundPlayer* player = reinterpret_cast<SoundPlayer*>(userdata);
    player->sdlAudioCallback(stream, len);
}

void SoundPlayer::sdlAudioCallback(unsigned char* stream, int byteLength) {
    memset(stream, 0, byteLength);
    if (!mSound) {
        return;
    }
    qreal playPosition = 0;
    {
        QMutexLocker lock(&mMutex);
        if (!mPlayThreadData.playing) {
            return;
        }
        int sampleCount = mPlayThreadData.samples.count();
        auto ptr = reinterpret_cast<qint16*>(stream);
        auto end = ptr + byteLength / 2;

        for (; ptr != end; ++mPlayThreadData.position, ++ptr) {
            if (mPlayThreadData.position == sampleCount) {
                mPlayThreadData.position = 0;
                if (!mPlayThreadData.loop) {
                    mPlayThreadData.playing = false;
                    break;
                }
            }
            qreal value = mPlayThreadData.samples[mPlayThreadData.position];
            *ptr = static_cast<qint16>(value * 32767);
        }
        playPosition = mPlayThreadData.position / qreal(sampleCount);
    }
    playPositionChanged(playPosition);
}

void SoundPlayer::registerCallback() {
    SDL_AudioSpec des;
    des.freq = 44100;
    des.format = AUDIO_S16SYS;
    des.channels = 1;
    des.samples = 512;
    des.callback = staticSdlAudioCallback;
    des.userdata = this;
    if (SDL_OpenAudio(&des, NULL) != 0) {
        fprintf(stderr, "Failed to init audio\n");
        exit(1);
    }
    SDL_PauseAudio(0);
}

void SoundPlayer::unregisterCallback() {
    SDL_CloseAudio();
}

void SoundPlayer::play() {
    mPlayTimer->start();
}

void SoundPlayer::onSoundModified() {
    updateSamples();
    play();
    soundModified();
}

void SoundPlayer::updateSamples() {
    QMutexLocker lock(&mMutex);

    mPlayThreadData.samples.clear();
    mPlayThreadData.position = 0;

    Synthesizer synth;
    synth.init(mSound);
    BufferStrategy strategy(&mPlayThreadData.samples);
    while (synth.synthSample(256, &strategy)) {
    }
}
