#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <memory>

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

    unsigned int mRandomSeed = 0;

    int mLastNoiseIndex = -1;
    float mLastNoiseValue = 0;

    void resetSample(bool restart);

    int rnd(int);
    float frnd(float range);

    float getNoise(float alpha);
};

#endif // SYNTHESIZER_H
