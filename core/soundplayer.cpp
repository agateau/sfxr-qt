#include "soundplayer.h"

#include <QTimer>

#include <SDL.h>

#include <sound.h>
#include <synthesizer.h>

static const int SCHEDULED_PLAY_DELAY = 200;

class BufferStrategy : public Synthesizer::SynthStrategy {
public:
    BufferStrategy(float* buffer)
        : buffer(buffer) {
    }
    void write(float ssample) override {
        *buffer++ = qBound(-1.f, ssample, 1.f);;
    }
private:
    float* buffer;
};

SoundPlayer::SoundPlayer(QObject* parent)
    : QObject(parent)
    , mPlayTimer(new QTimer(this)) {
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
    mSynth.reset(new Synthesizer(mSound));

    connect(mSound, &Sound::waveTypeChanged, this, &SoundPlayer::schedulePlay);

    connect(mSound, &Sound::attackTimeChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::sustainTimeChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::sustainPunchChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::decayTimeChanged, this, &SoundPlayer::schedulePlay);

    connect(mSound, &Sound::baseFrequencyChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::minFrequencyChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::slideChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::deltaSlideChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::vibratoDepthChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::vibratoSpeedChanged, this, &SoundPlayer::schedulePlay);

    connect(mSound, &Sound::changeAmountChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::changeSpeedChanged, this, &SoundPlayer::schedulePlay);

    connect(mSound, &Sound::squareDutyChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::dutySweepChanged, this, &SoundPlayer::schedulePlay);

    connect(mSound, &Sound::repeatSpeedChanged, this, &SoundPlayer::schedulePlay);

    connect(mSound, &Sound::phaserOffsetChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::phaserSweepChanged, this, &SoundPlayer::schedulePlay);

    connect(mSound, &Sound::lpFilterCutoffChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::lpFilterCutoffSweepChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::lpFilterResonanceChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::hpFilterCutoffChanged, this, &SoundPlayer::schedulePlay);
    connect(mSound, &Sound::hpFilterCutoffSweepChanged, this, &SoundPlayer::schedulePlay);

    connect(mSound, &Sound::volumeChanged, this, &SoundPlayer::schedulePlay);

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
    loopChanged(value);
}

void SoundPlayer::play() {
    mPlaying = true;
    mSynth->start();
}

void SoundPlayer::staticSdlAudioCallback(void* userdata, unsigned char* stream, int len) {
    SoundPlayer* player = reinterpret_cast<SoundPlayer*>(userdata);
    player->sdlAudioCallback(stream, len);
}

void SoundPlayer::sdlAudioCallback(unsigned char* stream, int len) {
    if (mPlaying) {
        unsigned int l = len / 2;
        float fbuf[l];
        memset(fbuf, 0, sizeof(fbuf));
        BufferStrategy strategy(fbuf);
        mPlaying = mSynth->synthSample(l, &strategy);
        while (l--) {
            float f = qBound(-1.f, fbuf[l], 1.f);
            ((Sint16*)stream)[l] = (Sint16)(f * 32767);
        }
        if (mLoop && !mPlaying) {
            play();
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

void SoundPlayer::schedulePlay() {
    mPlayTimer->start();
}
