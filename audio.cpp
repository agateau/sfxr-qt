#include "audio.h"

#include <stdio.h>

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

float master_vol=0.05f;

float sound_vol=0.5f;

bool playing_sample=false;
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

int wav_bits=16;
int wav_freq=44100;

void ResetParams()
{
	wave_type=0;

	p_base_freq=0.3f;
	p_freq_limit=0.0f;
	p_freq_ramp=0.0f;
	p_freq_dramp=0.0f;
	p_duty=0.0f;
	p_duty_ramp=0.0f;

	p_vib_strength=0.0f;
	p_vib_speed=0.0f;
	p_vib_delay=0.0f;

	p_env_attack=0.0f;
	p_env_sustain=0.3f;
	p_env_decay=0.4f;
	p_env_punch=0.0f;

	filter_on=false;
	p_lpf_resonance=0.0f;
	p_lpf_freq=1.0f;
	p_lpf_ramp=0.0f;
	p_hpf_freq=0.0f;
	p_hpf_ramp=0.0f;
	
	p_pha_offset=0.0f;
	p_pha_ramp=0.0f;

	p_repeat_speed=0.0f;

	p_arp_speed=0.0f;
	p_arp_mod=0.0f;
}

bool LoadSettings(char* filename)
{
	FILE* file=fopen(filename, "rb");
	if(!file)
		return false;

	int version=0;
	fread(&version, 1, sizeof(int), file);
	if(version!=100 && version!=101 && version!=102)
		return false;

	fread(&wave_type, 1, sizeof(int), file);

	sound_vol=0.5f;
	if(version==102)
		fread(&sound_vol, 1, sizeof(float), file);

	fread(&p_base_freq, 1, sizeof(float), file);
	fread(&p_freq_limit, 1, sizeof(float), file);
	fread(&p_freq_ramp, 1, sizeof(float), file);
	if(version>=101)
		fread(&p_freq_dramp, 1, sizeof(float), file);
	fread(&p_duty, 1, sizeof(float), file);
	fread(&p_duty_ramp, 1, sizeof(float), file);

	fread(&p_vib_strength, 1, sizeof(float), file);
	fread(&p_vib_speed, 1, sizeof(float), file);
	fread(&p_vib_delay, 1, sizeof(float), file);

	fread(&p_env_attack, 1, sizeof(float), file);
	fread(&p_env_sustain, 1, sizeof(float), file);
	fread(&p_env_decay, 1, sizeof(float), file);
	fread(&p_env_punch, 1, sizeof(float), file);

	fread(&filter_on, 1, sizeof(bool), file);
	fread(&p_lpf_resonance, 1, sizeof(float), file);
	fread(&p_lpf_freq, 1, sizeof(float), file);
	fread(&p_lpf_ramp, 1, sizeof(float), file);
	fread(&p_hpf_freq, 1, sizeof(float), file);
	fread(&p_hpf_ramp, 1, sizeof(float), file);
	
	fread(&p_pha_offset, 1, sizeof(float), file);
	fread(&p_pha_ramp, 1, sizeof(float), file);

	fread(&p_repeat_speed, 1, sizeof(float), file);

	if(version>=101)
	{
		fread(&p_arp_speed, 1, sizeof(float), file);
		fread(&p_arp_mod, 1, sizeof(float), file);
	}

	fclose(file);
	return true;
}

bool SaveSettings(char* filename)
{
	FILE* file=fopen(filename, "wb");
	if(!file)
		return false;

	int version=102;
	fwrite(&version, 1, sizeof(int), file);

	fwrite(&wave_type, 1, sizeof(int), file);

	fwrite(&sound_vol, 1, sizeof(float), file);

	fwrite(&p_base_freq, 1, sizeof(float), file);
	fwrite(&p_freq_limit, 1, sizeof(float), file);
	fwrite(&p_freq_ramp, 1, sizeof(float), file);
	fwrite(&p_freq_dramp, 1, sizeof(float), file);
	fwrite(&p_duty, 1, sizeof(float), file);
	fwrite(&p_duty_ramp, 1, sizeof(float), file);

	fwrite(&p_vib_strength, 1, sizeof(float), file);
	fwrite(&p_vib_speed, 1, sizeof(float), file);
	fwrite(&p_vib_delay, 1, sizeof(float), file);

	fwrite(&p_env_attack, 1, sizeof(float), file);
	fwrite(&p_env_sustain, 1, sizeof(float), file);
	fwrite(&p_env_decay, 1, sizeof(float), file);
	fwrite(&p_env_punch, 1, sizeof(float), file);

	fwrite(&filter_on, 1, sizeof(bool), file);
	fwrite(&p_lpf_resonance, 1, sizeof(float), file);
	fwrite(&p_lpf_freq, 1, sizeof(float), file);
	fwrite(&p_lpf_ramp, 1, sizeof(float), file);
	fwrite(&p_hpf_freq, 1, sizeof(float), file);
	fwrite(&p_hpf_ramp, 1, sizeof(float), file);
	
	fwrite(&p_pha_offset, 1, sizeof(float), file);
	fwrite(&p_pha_ramp, 1, sizeof(float), file);

	fwrite(&p_repeat_speed, 1, sizeof(float), file);

	fwrite(&p_arp_speed, 1, sizeof(float), file);
	fwrite(&p_arp_mod, 1, sizeof(float), file);

	fclose(file);
	return true;
}

