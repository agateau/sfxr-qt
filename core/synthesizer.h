#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <QObject>

class QTimer;

class Sound;

class Synthesizer : public QObject {
    Q_OBJECT
    Q_PROPERTY(Sound* sound READ sound WRITE setSound NOTIFY soundChanged)
public:
    class SynthStrategy {
    public:
        virtual ~SynthStrategy();
        virtual void write(float sample) = 0;
    };

    explicit Synthesizer(QObject* parent = nullptr);
    ~Synthesizer();

    Sound* sound() const;
    void setSound(Sound* value);

    bool exportWav(const QUrl& url);

    void play();
    void synthSample(int length, SynthStrategy* strategy);

signals:
    void soundChanged(Sound* value);

private:
    Sound* mSound = nullptr;

    bool playing_sample = false;

    // Internal
    int phase;
    double fperiod;
    double fmaxperiod;
    double fslide;
    double fdslide;
    int period;
    float square_duty;
    float square_slide;
    int env_stage;
    int env_time;
    int env_length[3];
    float env_vol;
    float fphase;
    float fdphase;
    int iphase;
    float phaser_buffer[1024];
    int ipp;
    float noise_buffer[32];
    float fltp;
    float fltdp;
    float fltw;
    float fltw_d;
    float fltdmp;
    float fltphp;
    float flthp;
    float flthp_d;
    float vib_phase;
    float vib_speed;
    float vib_amp;
    int rep_time;
    int rep_limit;
    int arp_time;
    int arp_limit;
    double arp_mod;

    bool mute_stream;

    bool mInited = false;

    QTimer* mPlayTimer;
    void init();
    void schedulePlay();
    void resetSample(bool restart);

    static void sdlAudioCallback(void* userdata, unsigned char* stream, int len);
};

#endif // SYNTHESIZER_H
