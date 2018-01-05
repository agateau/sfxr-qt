#ifndef AUDIO_H
#define AUDIO_H

extern int wave_type;

extern float p_base_freq;
extern float p_freq_limit;
extern float p_freq_ramp;
extern float p_freq_dramp;
extern float p_duty;
extern float p_duty_ramp;

extern float p_vib_strength;
extern float p_vib_speed;
extern float p_vib_delay;

extern float p_env_attack;
extern float p_env_sustain;
extern float p_env_decay;
extern float p_env_punch;

extern bool filter_on;
extern float p_lpf_resonance;
extern float p_lpf_freq;
extern float p_lpf_ramp;
extern float p_hpf_freq;
extern float p_hpf_ramp;

extern float p_pha_offset;
extern float p_pha_ramp;

extern float p_repeat_speed;

extern float p_arp_speed;
extern float p_arp_mod;

extern float master_vol;

extern float sound_vol;

extern bool playing_sample;
extern int phase;
extern double fperiod;
extern double fmaxperiod;
extern double fslide;
extern double fdslide;
extern int period;
extern float square_duty;
extern float square_slide;
extern int env_stage;
extern int env_time;
extern int env_length[3];
extern float env_vol;
extern float fphase;
extern float fdphase;
extern int iphase;
extern float phaser_buffer[1024];
extern int ipp;
extern float noise_buffer[32];
extern float fltp;
extern float fltdp;
extern float fltw;
extern float fltw_d;
extern float fltdmp;
extern float fltphp;
extern float flthp;
extern float flthp_d;
extern float vib_phase;
extern float vib_speed;
extern float vib_amp;
extern int rep_time;
extern int rep_limit;
extern int arp_time;
extern int arp_limit;
extern double arp_mod;

extern int wav_bits;
extern int wav_freq;

void ResetParams();

#endif /* AUDIO_H */
