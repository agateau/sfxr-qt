#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include "NoiseGenerator.h"

#include <memory>
#include <unordered_map>

static constexpr int PHASER_BUFFER_LENGTH = 1024;

class Sound;

class Synthesizer {
public:
    class SynthStrategy {
    public:
        virtual ~SynthStrategy();
        virtual void write(float sample) = 0;
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
    double fperiod;
    double fmaxperiod;
    double fslide;
    double fdslide;
    float square_duty;
    float square_slide;
    EnvelopStage env_stage;
    int env_time;
    std::unordered_map<EnvelopStage, int> env_length;
    float env_vol;
    float fphase;
    float fdphase;
    float phaser_buffer[PHASER_BUFFER_LENGTH];
    int ipp;
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

    void resetSample(bool restart);

    NoiseGenerator mNoiseGenerator;
};

#endif // SYNTHESIZER_H
