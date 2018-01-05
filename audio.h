#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>

#include <SDL.h>

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

extern float sound_vol;

extern bool playing_sample;

extern int wav_bits;
extern int wav_freq;

void ResetParams();
bool LoadSettings(char* filename);
bool SaveSettings(char* filename);

void ResetSample(bool restart);
void PlaySample();
void SynthSample(int length, float* buffer, FILE* file);
bool ExportWAV(char* filename);
void InitSDLAudio();

float frnd(float range);

#define rnd(n) (rand()%(n+1))

#endif /* AUDIO_H */
