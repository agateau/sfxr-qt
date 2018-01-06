#include "sdlkit.h"

#include <gtk/gtk.h>
#include <string.h>
#include <malloc.h>

Uint32 *ddkscreen32;
Uint16 *ddkscreen16;
int ddkpitch;
int mouse_x, mouse_y, mouse_px, mouse_py;
bool mouse_left = false, mouse_right = false, mouse_middle = false;
bool mouse_leftclick = false, mouse_rightclick = false, mouse_middleclick = false;
Spriteset font;

static SDL_Surface *sdlscreen = NULL;

static bool keys[SDLK_LAST];

bool DPInput::KeyPressed(SDLKey key)
{
	bool r = keys[key];
	keys[key] = false;
	return r;
}

void error (const char *file, unsigned int line, const char *msg)
{
	fprintf(stderr, "[!] %s:%u  %s\n", file, line, msg);
	exit(1);
}


bool load_file (char *fname)
{
	char *fn;

	GtkWidget *dialog = gtk_file_chooser_dialog_new("Load a file!",
	                                                NULL,
	                                                GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
	                                                NULL
	                                                );

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		strncpy(fname, fn, 255);
		g_free(fn);

		fname[255] = 0;
	}

	gtk_widget_destroy(dialog);

	while (gtk_events_pending ()) gtk_main_iteration ();

	return true;
}

bool save_file (char *fname)
{
	char *fn;

	GtkWidget *dialog = gtk_file_chooser_dialog_new("Save a file!",
	                                                NULL,
	                                                GTK_FILE_CHOOSER_ACTION_SAVE,
	                                                GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
	                                                NULL
	                                                );

	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		strncpy(fname, fn, 255);
		g_free(fn);

		fname[255] = 0;
	}

	gtk_widget_destroy(dialog);

	while (gtk_events_pending ()) gtk_main_iteration ();

	return true;
}

static void sdlquit ()
{
	ddkFree();
	SDL_Quit();
}

static void sdlinit ()
{
	SDL_Surface *icon;
	VERIFY(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));
	icon = SDL_LoadBMP("/usr/share/sfxr/sfxr.bmp");
	if (!icon)
		icon = SDL_LoadBMP("sfxr.bmp");
	if (icon)
		SDL_WM_SetIcon(icon, NULL);
	atexit(sdlquit);
	memset(keys, 0, sizeof(keys));
	ddkInit();
}

static void sdlupdate ()
{
	mouse_px = mouse_x;
	mouse_py = mouse_y;
	Uint8 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
	bool mouse_left_p = mouse_left;
	bool mouse_right_p = mouse_right;
	bool mouse_middle_p = mouse_middle;
	mouse_left = buttons & SDL_BUTTON(1);
	mouse_right = buttons & SDL_BUTTON(3);
	mouse_middle = buttons & SDL_BUTTON(2);
	mouse_leftclick = mouse_left && !mouse_left_p;
	mouse_rightclick = mouse_right && !mouse_right_p;
	mouse_middleclick = mouse_middle && !mouse_middle_p;
}

static void loop (void)
{
	SDL_Event e;
	while (true)
	{
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT:
					return;
	
				case SDL_KEYDOWN:
					keys[e.key.keysym.sym] = true;
	
				default: break;
			}
		}

		sdlupdate();

		if (!ddkCalcFrame())
			return;

		SDL_Flip(sdlscreen);
	}
}

bool ddkLock ()
{
	SDL_LockSurface(sdlscreen);
	ddkpitch = sdlscreen->pitch / (sdlscreen->format->BitsPerPixel == 32 ? 4 : 2);
	ddkscreen16 = (Uint16*)(sdlscreen->pixels);
	ddkscreen32 = (Uint32*)(sdlscreen->pixels);
}

void ddkUnlock ()
{
	SDL_UnlockSurface(sdlscreen);
}

void ddkSetMode (int width, int height, int bpp, int refreshrate, int fullscreen, const char *title)
{
	VERIFY(sdlscreen = SDL_SetVideoMode(width, height, bpp, fullscreen ? SDL_FULLSCREEN : 0));
	SDL_WM_SetCaption(title, title);
}

int LoadTGA(Spriteset& tiles, const char *filename)
{
	FILE *file;
	unsigned char byte, crap[16], id_length;
	int n, width, height, channels, x, y;
	file=fopen(filename, "rb");
	if (!file)
		return -1;
	fread(&id_length, 1, 1, file);
	fread(crap, 1, 11, file);
	width=0;
	height=0;
	fread(&width, 1, 2, file);	// width
	fread(&height, 1, 2, file);	// height
	fread(&byte, 1, 1, file);	// bits
	channels=byte/8;
	fread(&byte, 1, 1, file);	// image descriptor byte (per-bit info)
	for(n=0;n<id_length;n++)
		fread(&byte, 1, 1, file);	// image description
	tiles.data=(DWORD*)malloc(width*height*sizeof(DWORD));
	for(y=height-1;y>=0;y--)
		for(x=0;x<width;x++)
		{
			DWORD pixel=0;
			fread(&byte, 1, 1, file);
			pixel|=byte;
			fread(&byte, 1, 1, file);
			pixel|=byte<<8;
			fread(&byte, 1, 1, file);
			pixel|=byte<<16;
			tiles.data[y*width+x]=pixel;
		}
	fclose(file);
	tiles.height=height;
	tiles.width=height;
	tiles.pitch=width;

	return 0;
}

void ClearScreen(DWORD color)
{
	for(int y=0;y<480;y++)
	{
		int offset=y*ddkpitch;
		for(int x=0;x<640;x+=8)
		{
			ddkscreen32[offset++]=color;
			ddkscreen32[offset++]=color;
			ddkscreen32[offset++]=color;
			ddkscreen32[offset++]=color;
			ddkscreen32[offset++]=color;
			ddkscreen32[offset++]=color;
			ddkscreen32[offset++]=color;
			ddkscreen32[offset++]=color;
		}
	}
}

void DrawBar(int sx, int sy, int w, int h, DWORD color)
{
	for(int y=sy;y<sy+h;y++)
	{
		int offset=y*ddkpitch+sx;
		int x1=0;
		if(w>8)
			for(x1=0;x1<w-8;x1+=8)
			{
				ddkscreen32[offset++]=color;
				ddkscreen32[offset++]=color;
				ddkscreen32[offset++]=color;
				ddkscreen32[offset++]=color;
				ddkscreen32[offset++]=color;
				ddkscreen32[offset++]=color;
				ddkscreen32[offset++]=color;
				ddkscreen32[offset++]=color;
			}
		for(int x=x1;x<w;x++)
			ddkscreen32[offset++]=color;
	}
}

void DrawBox(int sx, int sy, int w, int h, DWORD color)
{
	DrawBar(sx, sy, w, 1, color);
	DrawBar(sx, sy, 1, h, color);
	DrawBar(sx+w, sy, 1, h, color);
	DrawBar(sx, sy+h, w+1, 1, color);
}

void DrawSprite(Spriteset& sprites, int sx, int sy, int i, DWORD color)
{
	for(int y=0;y<sprites.height;y++)
	{
		int offset=(sy+y)*ddkpitch+sx;
		int spoffset=y*sprites.pitch+i*sprites.width;
		if(color&0xFF000000)
			for(int x=0;x<sprites.width;x++)
			{
				DWORD p=sprites.data[spoffset++];
				if(p!=0x300030)
					ddkscreen32[offset+x]=p;
			}
		else
			for(int x=0;x<sprites.width;x++)
			{
				DWORD p=sprites.data[spoffset++];
				if(p!=0x300030)
					ddkscreen32[offset+x]=color;
			}
	}
}

void DrawText(int sx, int sy, DWORD color, const char *string, ...)
{
	char string2[256];
	va_list args;

	va_start(args, string);
	vsprintf(string2, string, args);
	va_end(args);

	int len=strlen(string2);
	for(int i=0;i<len;i++)
		DrawSprite(font, sx+i*8, sy, string2[i]-' ', color);
}

bool MouseInBox(int x, int y, int w, int h)
{
	if(mouse_x>=x && mouse_x<x+w && mouse_y>=y && mouse_y<y+h)
		return true;
	return false;
}


int main (int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	sdlinit();
	loop();
	return 0;
}
