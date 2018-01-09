#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

class Sound;

class Synthesizer {
public:
    class SynthStrategy {
    public:
        virtual ~SynthStrategy();
        virtual void write(float sample) = 0;
    };

    explicit Synthesizer(const Sound* sound);
    ~Synthesizer();

    void start();
    bool synthSample(int length, SynthStrategy* strategy);

private:
    const Sound* mSound = nullptr;

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

    void resetSample(bool restart);
};

#endif // SYNTHESIZER_H
