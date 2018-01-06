/*

   Porting: Search for WIN32 to find windows-specific code
            snippets which need to be replaced or removed.

*/

#include "sdlkit.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "SDL.h"

#include "audio.h"

Spriteset ld48;

struct Category
{
	char name[32];
};

Category categories[10];

float* vselected=NULL;
int vcurbutton=-1;

DPInput *input;

bool firstframe=true;
int refresh_counter=0;

void Slider(int x, int y, float& value, bool bipolar, const char* text)
{
	if(MouseInBox(x, y, 100, 10))
	{
		if(mouse_leftclick)
			vselected=&value;
		if(mouse_rightclick)
			value=0.0f;
	}
	float mv=(float)(mouse_x-mouse_px);
	if(vselected!=&value)
		mv=0.0f;
	if(bipolar)
	{
		value+=mv*0.005f;
		if(value<-1.0f) value=-1.0f;
		if(value>1.0f) value=1.0f;
	}
	else
	{
		value+=mv*0.0025f;
		if(value<0.0f) value=0.0f;
		if(value>1.0f) value=1.0f;
	}
	DrawBar(x-1, y, 102, 10, 0x000000);
	int ival=(int)(value*99);
	if(bipolar)
		ival=(int)(value*49.5f+49.5f);
	DrawBar(x, y+1, ival, 8, 0xF0C090);
	DrawBar(x+ival, y+1, 100-ival, 8, 0x807060);
	DrawBar(x+ival, y+1, 1, 8, 0xFFFFFF);
	if(bipolar)
	{
		DrawBar(x+50, y-1, 1, 3, 0x000000);
		DrawBar(x+50, y+8, 1, 3, 0x000000);
	}
	DWORD tcol=0x000000;
	if(wave_type!=0 && (&value==&p_duty || &value==&p_duty_ramp))
		tcol=0x808080;
	DrawText(x-4-strlen(text)*8, y+1, tcol, text);
}

bool Button(int x, int y, bool highlight, const char* text, int id)
{
	DWORD color1=0x000000;
	DWORD color2=0xA09088;
	DWORD color3=0x000000;
	bool hover=MouseInBox(x, y, 100, 17);
	if(hover && mouse_leftclick)
		vcurbutton=id;
	bool current=(vcurbutton==id);
	if(highlight)
	{
		color1=0x000000;
		color2=0x988070;
		color3=0xFFF0E0;
	}
	if(current && hover)
	{
		color1=0xA09088;
		color2=0xFFF0E0;
		color3=0xA09088;
	}
	DrawBar(x-1, y-1, 102, 19, color1);
	DrawBar(x, y, 100, 17, color2);
	DrawText(x+5, y+5, color3, text);
	if(current && hover && !mouse_left)
		return true;
	return false;
}

int drawcount=0;

void DrawScreen()
{
	bool redraw=true;
	if(!firstframe && mouse_x-mouse_px==0 && mouse_y-mouse_py==0 && !mouse_left && !mouse_right)
		redraw=false;
	if(!mouse_left)
	{
		if(vselected!=NULL || vcurbutton>-1)
		{
			redraw=true;
			refresh_counter=2;
		}
		vselected=NULL;
	}
	if(refresh_counter>0)
	{
		refresh_counter--;
		redraw=true;
	}

	if(playing_sample)
		redraw=true;

	if(drawcount++>20)
	{
		redraw=true;
		drawcount=0;
	}

	if(!redraw)
		return;

	firstframe=false;

	ddkLock();

	ClearScreen(0xC0B090);

	DrawText(10, 10, 0x504030, "GENERATOR");
	for(int i=0;i<7;i++)
	{
		if(Button(5, 35+i*30, false, categories[i].name, 300+i))
		{
			switch(i)
			{
			case 0: // pickup/coin
				PickupCoin();
				break;
			case 1: // laser/shoot
                LaserShoot();
				break;
			case 2: // explosion
                Explosion();
				break;
			case 3: // powerup
                Powerup();
				break;
			case 4: // hit/hurt
                HitHurt();
				break;
			case 5: // jump
                Jump();
				break;
			case 6: // blip/select
                BlipSelect();
				break;
			default:
				break;
			}

			PlaySample();
		}
	}
	DrawBar(110, 0, 2, 480, 0x000000);
	DrawText(120, 10, 0x504030, "MANUAL SETTINGS");
	DrawSprite(ld48, 8, 440, 0, 0xB0A080);

	if(Button(130, 30, wave_type==0, "SQUAREWAVE", 10))
		wave_type=0;
	if(Button(250, 30, wave_type==1, "SAWTOOTH", 11))
		wave_type=1;
	if(Button(370, 30, wave_type==2, "SINEWAVE", 12))
		wave_type=2;
	if(Button(490, 30, wave_type==3, "NOISE", 13))
		wave_type=3;

	bool do_play=false;

	DrawBar(5-1-1, 412-1-1, 102+2, 19+2, 0x000000);
	if(Button(5, 412, false, "RANDOMIZE", 40))
	{
		p_base_freq=pow(frnd(2.0f)-1.0f, 2.0f);
		if(rnd(1))
			p_base_freq=pow(frnd(2.0f)-1.0f, 3.0f)+0.5f;
		p_freq_limit=0.0f;
		p_freq_ramp=pow(frnd(2.0f)-1.0f, 5.0f);
		if(p_base_freq>0.7f && p_freq_ramp>0.2f)
			p_freq_ramp=-p_freq_ramp;
		if(p_base_freq<0.2f && p_freq_ramp<-0.05f)
			p_freq_ramp=-p_freq_ramp;
		p_freq_dramp=pow(frnd(2.0f)-1.0f, 3.0f);
		p_duty=frnd(2.0f)-1.0f;
		p_duty_ramp=pow(frnd(2.0f)-1.0f, 3.0f);
		p_vib_strength=pow(frnd(2.0f)-1.0f, 3.0f);
		p_vib_speed=frnd(2.0f)-1.0f;
		p_vib_delay=frnd(2.0f)-1.0f;
		p_env_attack=pow(frnd(2.0f)-1.0f, 3.0f);
		p_env_sustain=pow(frnd(2.0f)-1.0f, 2.0f);
		p_env_decay=frnd(2.0f)-1.0f;
		p_env_punch=pow(frnd(0.8f), 2.0f);
		if(p_env_attack+p_env_sustain+p_env_decay<0.2f)
		{
			p_env_sustain+=0.2f+frnd(0.3f);
			p_env_decay+=0.2f+frnd(0.3f);
		}
		p_lpf_resonance=frnd(2.0f)-1.0f;
		p_lpf_freq=1.0f-pow(frnd(1.0f), 3.0f);
		p_lpf_ramp=pow(frnd(2.0f)-1.0f, 3.0f);
		if(p_lpf_freq<0.1f && p_lpf_ramp<-0.05f)
			p_lpf_ramp=-p_lpf_ramp;
		p_hpf_freq=pow(frnd(1.0f), 5.0f);
		p_hpf_ramp=pow(frnd(2.0f)-1.0f, 5.0f);
		p_pha_offset=pow(frnd(2.0f)-1.0f, 3.0f);
		p_pha_ramp=pow(frnd(2.0f)-1.0f, 3.0f);
		p_repeat_speed=frnd(2.0f)-1.0f;
		p_arp_speed=frnd(2.0f)-1.0f;
		p_arp_mod=frnd(2.0f)-1.0f;
		do_play=true;
	}

	if(Button(5, 382, false, "MUTATE", 30))
	{
		if(rnd(1)) p_base_freq+=frnd(0.1f)-0.05f;
//		if(rnd(1)) p_freq_limit+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_freq_ramp+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_freq_dramp+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_duty+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_duty_ramp+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_vib_strength+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_vib_speed+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_vib_delay+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_env_attack+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_env_sustain+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_env_decay+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_env_punch+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_lpf_resonance+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_lpf_freq+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_lpf_ramp+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_hpf_freq+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_hpf_ramp+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_pha_offset+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_pha_ramp+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_repeat_speed+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_arp_speed+=frnd(0.1f)-0.05f;
		if(rnd(1)) p_arp_mod+=frnd(0.1f)-0.05f;
		do_play=true;
	}

	DrawText(515, 170, 0x000000, "VOLUME");
	DrawBar(490-1-1+60, 180-1+5, 70, 2, 0x000000);
	DrawBar(490-1-1+60+68, 180-1+5, 2, 205, 0x000000);
	DrawBar(490-1-1+60, 180-1, 42+2, 10+2, 0xFF0000);
	Slider(490, 180, sound_vol, false, " ");
	if(Button(490, 200, false, "PLAY SOUND", 20))
		PlaySample();

	if(Button(490, 290, false, "LOAD SOUND", 14))
	{
		char filename[256];
		if(FileSelectorLoad(hWndMain, filename, 1)) // WIN32
		{
			ResetParams();
			LoadSettings(filename);
			PlaySample();
		}
	}
	if(Button(490, 320, false, "SAVE SOUND", 15))
	{
		char filename[256];
		if(FileSelectorSave(hWndMain, filename, 1)) // WIN32
			SaveSettings(filename);
	}

	DrawBar(490-1-1+60, 380-1+9, 70, 2, 0x000000);
	DrawBar(490-1-2, 380-1-2, 102+4, 19+4, 0x000000);
	if(Button(490, 380, false, "EXPORT .WAV", 16))
	{
		char filename[256];
		if(FileSelectorSave(hWndMain, filename, 0)) // WIN32
			ExportWAV(filename);
	}
	char str[10];
	sprintf(str, "%i HZ", wav_freq);
	if(Button(490, 410, false, str, 18))
	{
		if(wav_freq==44100)
			wav_freq=22050;
		else
			wav_freq=44100;
	}
	sprintf(str, "%i-BIT", wav_bits);
	if(Button(490, 440, false, str, 19))
	{
		if(wav_bits==16)
			wav_bits=8;
		else
			wav_bits=16;
	}

	int ypos=4;

	int xpos=350;

	DrawBar(xpos-190, ypos*18-5, 300, 2, 0x0000000);

	Slider(xpos, (ypos++)*18, p_env_attack, false, "ATTACK TIME");
	Slider(xpos, (ypos++)*18, p_env_sustain, false, "SUSTAIN TIME");
	Slider(xpos, (ypos++)*18, p_env_punch, false, "SUSTAIN PUNCH");
	Slider(xpos, (ypos++)*18, p_env_decay, false, "DECAY TIME");

	DrawBar(xpos-190, ypos*18-5, 300, 2, 0x0000000);

	Slider(xpos, (ypos++)*18, p_base_freq, false, "START FREQUENCY");
	Slider(xpos, (ypos++)*18, p_freq_limit, false, "MIN FREQUENCY");
	Slider(xpos, (ypos++)*18, p_freq_ramp, true, "SLIDE");
	Slider(xpos, (ypos++)*18, p_freq_dramp, true, "DELTA SLIDE");

	Slider(xpos, (ypos++)*18, p_vib_strength, false, "VIBRATO DEPTH");
	Slider(xpos, (ypos++)*18, p_vib_speed, false, "VIBRATO SPEED");

	DrawBar(xpos-190, ypos*18-5, 300, 2, 0x0000000);

	Slider(xpos, (ypos++)*18, p_arp_mod, true, "CHANGE AMOUNT");
	Slider(xpos, (ypos++)*18, p_arp_speed, false, "CHANGE SPEED");

	DrawBar(xpos-190, ypos*18-5, 300, 2, 0x0000000);

	Slider(xpos, (ypos++)*18, p_duty, false, "SQUARE DUTY");
	Slider(xpos, (ypos++)*18, p_duty_ramp, true, "DUTY SWEEP");

	DrawBar(xpos-190, ypos*18-5, 300, 2, 0x0000000);

	Slider(xpos, (ypos++)*18, p_repeat_speed, false, "REPEAT SPEED");

	DrawBar(xpos-190, ypos*18-5, 300, 2, 0x0000000);

	Slider(xpos, (ypos++)*18, p_pha_offset, true, "PHASER OFFSET");
	Slider(xpos, (ypos++)*18, p_pha_ramp, true, "PHASER SWEEP");

	DrawBar(xpos-190, ypos*18-5, 300, 2, 0x0000000);

	Slider(xpos, (ypos++)*18, p_lpf_freq, false, "LP FILTER CUTOFF");
	Slider(xpos, (ypos++)*18, p_lpf_ramp, true, "LP FILTER CUTOFF SWEEP");
	Slider(xpos, (ypos++)*18, p_lpf_resonance, false, "LP FILTER RESONANCE");
	Slider(xpos, (ypos++)*18, p_hpf_freq, false, "HP FILTER CUTOFF");
	Slider(xpos, (ypos++)*18, p_hpf_ramp, true, "HP FILTER CUTOFF SWEEP");

	DrawBar(xpos-190, ypos*18-5, 300, 2, 0x0000000);

	DrawBar(xpos-190, 4*18-5, 1, (ypos-4)*18, 0x0000000);
	DrawBar(xpos-190+299, 4*18-5, 1, (ypos-4)*18, 0x0000000);

	if(do_play)
		PlaySample();

	ddkUnlock();

	if(!mouse_left)
		vcurbutton=-1;
}

bool keydown=false;

bool ddkCalcFrame()
{
	input->Update(); // WIN32 (for keyboard input)

	if(input->KeyPressed(DIK_SPACE) || input->KeyPressed(DIK_RETURN)) // WIN32 (keyboard input only for convenience, ok to remove)
	{
		if(!keydown)
		{
			PlaySample();
			keydown=true;
		}
	}
	else
		keydown=false;

	DrawScreen();

	Sleep(5); // WIN32
	return true;
}

void ddkInit()
{
	srand(time(NULL));

	ddkSetMode(640,480, 32, 60, DDK_WINDOW, "sfxr"); // requests window size etc from ddrawkit

	if (LoadTGA(font, "/usr/share/sfxr/font.tga")) {
        	/* Try again in cwd */
		if (LoadTGA(font, "font.tga")) {
			fprintf(stderr,
				"Error could not open /usr/share/sfxr/font.tga"
				" nor font.tga\n");
			exit(1);
		}
	}

	if (LoadTGA(ld48, "/usr/share/sfxr/ld48.tga")) {
        	/* Try again in cwd */
		if (LoadTGA(ld48, "ld48.tga")) {
			fprintf(stderr,
				"Error could not open /usr/share/sfxr/ld48.tga"
				" nor ld48.tga\n");
			exit(1);
		}
	}

	ld48.width=ld48.pitch;

	input=new DPInput(hWndMain, hInstanceMain); // WIN32

	strcpy(categories[0].name, "PICKUP/COIN");
	strcpy(categories[1].name, "LASER/SHOOT");
	strcpy(categories[2].name, "EXPLOSION");
	strcpy(categories[3].name, "POWERUP");
	strcpy(categories[4].name, "HIT/HURT");
	strcpy(categories[5].name, "JUMP");
	strcpy(categories[6].name, "BLIP/SELECT");

	ResetParams();
	InitSDLAudio();
}

void ddkFree()
{
	delete input;
	free(ld48.data);
	free(font.data);
}

