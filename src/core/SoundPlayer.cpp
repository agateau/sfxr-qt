#include "SoundPlayer.h"

#include "Sound.h"
#include "Synthesizer.h"

#include <QTimer>

#include <SDL.h>

static const int SCHEDULED_PLAY_DELAY = 200;

class BufferStrategy : public Synthesizer::SynthStrategy {
public:
    BufferStrategy(qreal* buffer) : buffer(buffer) {
    }
    void write(qreal ssample) override {
        *buffer++ = qBound(-1., ssample, 1.);
    }

private:
    qreal* buffer;
};

SoundPlayer::SoundPlayer(QObject* parent) : QObject(parent), mPlayTimer(new QTimer(this)) {
    mPlayThreadData.synth.reset(new Synthesizer);

    mPlayTimer->setInterval(SCHEDULED_PLAY_DELAY);
    mPlayTimer->setSingleShot(true);
    connect(mPlayTimer, &QTimer::timeout, this, &SoundPlayer::play);
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
        connect(mSound, &Sound::modified, this, &SoundPlayer::onSoundModified);
    } else {
        mPlaying = false;
    }
    {
        QMutexLocker lock(&mMutex);
        mPlayThreadData.playing = mPlaying;
        if (mSound) {
            mPlayThreadData.synth->init(mSound);
        }
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

void SoundPlayer::play() {
    mPlaying = true;
    {
        QMutexLocker lock(&mMutex);
        mPlayThreadData.playing = mPlaying;
        mPlayThreadData.synth->start();
    }
}

void SoundPlayer::staticSdlAudioCallback(void* userdata, unsigned char* stream, int len) {
    SoundPlayer* player = reinterpret_cast<SoundPlayer*>(userdata);
    player->sdlAudioCallback(stream, len);
}

void SoundPlayer::sdlAudioCallback(unsigned char* stream, int len) {
    QMutexLocker lock(&mMutex);
    if (mPlayThreadData.playing) {
        unsigned int l = len / 2;
        qreal fbuf[l];
        memset(fbuf, 0, sizeof(fbuf));
        BufferStrategy strategy(fbuf);
        mPlayThreadData.playing = mPlayThreadData.synth->synthSample(l, &strategy);
        while (l--) {
            qreal f = qBound(-1., fbuf[l], 1.);
            ((Sint16*)stream)[l] = (Sint16)(f * 32767);
        }
        if (mPlayThreadData.loop && !mPlayThreadData.playing) {
            mPlayThreadData.playing = true;
            mPlayThreadData.synth->start();
        }
    } else {
        memset(stream, 0, len);
    }
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

void SoundPlayer::onSoundModified() {
    {
        QMutexLocker lock(&mMutex);
        mPlayThreadData.synth->init(mSound);
    }
    schedulePlay();
}

void SoundPlayer::schedulePlay() {
    mPlayTimer->start();
}
