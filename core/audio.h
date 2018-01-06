#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>

class Audio {
public:
    void Init();

    // Params
    void ResetParams();
    bool LoadSettings(char* filename);
    bool SaveSettings(char* filename);

    // Play
    void ResetSample(bool restart);
    void PlaySample();
    void SynthSample(int length, float* buffer, FILE* file);
    bool ExportWAV(char* filename);

    // High level
    void PickupCoin();
    void LaserShoot();
    void Explosion();
    void Powerup();
    void HitHurt();
    void Jump();
    void BlipSelect();

    void playCallback(unsigned char *stream, int len);

private:
    // Fields editable from the outside
    int wave_type;

    float p_base_freq;
    float p_freq_limit;
    float p_freq_ramp;
    float p_freq_dramp;
    float p_duty;
    float p_duty_ramp;

    float p_vib_strength;
    float p_vib_speed;
    float p_vib_delay;

    float p_env_attack;
    float p_env_sustain;
    float p_env_decay;
    float p_env_punch;

    bool filter_on;
    float p_lpf_resonance;
    float p_lpf_freq;
    float p_lpf_ramp;
    float p_hpf_freq;
    float p_hpf_ramp;

    float p_pha_offset;
    float p_pha_ramp;

    float p_repeat_speed;

    float p_arp_speed;
    float p_arp_mod;

    float sound_vol = 0.5f;

    bool playing_sample = false;

    int wav_bits = 16;
    int wav_freq = 44100;

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
    int file_sampleswritten;
    float filesample=0.0f;
    int fileacc=0;
};

#endif /* AUDIO_H */
