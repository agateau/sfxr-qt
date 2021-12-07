#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <QtGlobal>
#include <memory>
#include <unordered_map>

static constexpr int PHASER_BUFFER_LENGTH = 1024;

class WaveFormGenerator;
class Sound;

class Synthesizer {
public:
    class SynthStrategy {
    public:
        virtual ~SynthStrategy();
        virtual void write(qreal sample) = 0;
    };

    Synthesizer();
    ~Synthesizer();

    void init(const Sound* sound);
    void start();
    bool synthSample(int length, SynthStrategy* strategy);

private:
    std::unique_ptr<Sound> mSound;
    enum EnvelopStage {
        Attack,
        Sustain,
        Decay,
    };

    // Internal
    int phase;
    qreal fperiod;
    qreal fmaxperiod;
    qreal fslide;
    qreal fdslide;
    qreal square_duty;
    EnvelopStage env_stage;
    int env_time;
    std::unordered_map<EnvelopStage, int> env_length;
    qreal env_vol;
    qreal fphase;
    qreal fdphase;
    qreal phaser_buffer[PHASER_BUFFER_LENGTH];
    int ipp;
    qreal fltp;
    qreal fltdp;
    qreal fltw;
    qreal fltw_d;
    qreal fltdmp;
    qreal fltphp;
    qreal flthp;
    qreal flthp_d;
    qreal vib_phase;
    qreal vib_speed;
    qreal vib_amp;
    int rep_time;
    int rep_limit;
    int arp_time;
    int arp_limit;
    qreal arp_mod;

    void resetSample(bool restart);

    std::unique_ptr<WaveFormGenerator> mWaveFormGenerator;
};

#endif // SYNTHESIZER_H
