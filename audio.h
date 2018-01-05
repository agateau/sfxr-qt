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

void ResetParams();

#endif /* AUDIO_H */
