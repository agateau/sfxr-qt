#ifndef SDLKIT_H
#define SDLKIT_H

#include "SDL.h"
#define ERROR(x) error(__FILE__, __LINE__, #x)
#define VERIFY(x) do { if (!(x)) ERROR(x); } while (0)
#include <stdio.h>
#include <string.h>

void error (const char *file, unsigned int line, const char *msg);

typedef Uint32 DWORD;
typedef Uint16 WORD;

#define DIK_SPACE SDLK_SPACE
#define DIK_RETURN SDLK_RETURN
#define DDK_WINDOW 0

#define hWndMain 0
#define hInstanceMain 0

#define Sleep(x) SDL_Delay(x)

void ddkInit();      // Will be called on startup
bool ddkCalcFrame(); // Will be called every frame, return true to continue running or false to quit
void ddkFree();      // Will be called on shutdown

bool load_file (char *fname);
bool save_file (char *fname);

#define FileSelectorLoad(x,file,y) load_file(file)
#define FileSelectorSave(x,file,y) save_file(file)

class DPInput {
public:
	DPInput(int,int) {}
	~DPInput() {}
	static void Update () {}

	static bool KeyPressed(SDLKey key);
};

struct Spriteset
{
	DWORD *data;
	int width;
	int height;
	int pitch;
};

bool ddkLock ();
void ddkUnlock ();

void ddkSetMode (int width, int height, int bpp, int refreshrate, int fullscreen, const char *title);

extern Uint32 *ddkscreen32;
extern Uint16 *ddkscreen16;
extern int ddkpitch;
extern int mouse_x;
extern int mouse_y;
extern int mouse_px;
extern int mouse_py;
extern bool mouse_left;
extern bool mouse_right;
extern bool mouse_middle;
extern bool mouse_leftclick;
extern bool mouse_rightclick;
extern bool mouse_middleclick;
extern Spriteset font;

int LoadTGA(Spriteset& tiles, const char *filename);
void ClearScreen(DWORD color);
void DrawBar(int sx, int sy, int w, int h, DWORD color);
void DrawBox(int sx, int sy, int w, int h, DWORD color);
void DrawSprite(Spriteset& sprites, int sx, int sy, int i, DWORD color);
void DrawText(int sx, int sy, DWORD color, const char *string, ...);
bool MouseInBox(int x, int y, int w, int h);

#endif
