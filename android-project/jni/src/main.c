#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>

#include "common/sound.h"
#include "common/memorymap.h"
#include "common/supervision.h"
#include "common/controls.h"
#include "common/types.h"

#include "SDL.h"

#define SCALE_TO 480

BOOL paused = FALSE;

uint8* buffer;
unsigned int buffer_size = 0;

unsigned short screenbuffer[161*161];
SDL_Surface *PrimarySurface;
//SDL_Surface *ScreenSurface;
SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture *sdlTexture;
SDL_Rect stretchRect;

char *romname;

void loadROM(char* filename)
{
    SDL_Log("loading rom %s", filename);
	u32 length;

	if (buffer != 0)
		free(buffer);

	SDL_RWops *romfile = SDL_RWFromFile(filename, "rb");
	
	if (romfile == NULL)
		SDL_Log("fopen(): Unable to open file!\n");

	SDL_RWseek(romfile, 0, SEEK_END);
	buffer_size = SDL_RWtell(romfile);
	SDL_RWseek(romfile, 0, SEEK_SET);

	buffer = (unsigned char *)malloc(buffer_size);

	uint32 bytesread = SDL_RWread(romfile, buffer, buffer_size, 1);

	if (!SDL_RWclose(romfile))
		SDL_Log("fclose(): Unable to close file!\n");
}

void Draw()
{
/*
 *  Transfer the contents of work frame on the screen
 *
 */
	uint16 *pDest, *pSrc;
	int x, y;

	SDL_LockSurface(PrimarySurface);
	pDest = (uint16 *)PrimarySurface->pixels;
	pSrc = screenbuffer;

	/* Blit screen */
	for ( y = 0; y < 160; y++ )
	{
		for ( x = 0; x < 160; x++ )
		{
			*pDest = (((*pSrc) & 0x7FE0) << 1) | ((*pSrc) & 0x001F);
			pDest++;pSrc++;
		}
	}
	SDL_UnlockSurface(PrimarySurface);
	
    SDL_UpdateTexture(sdlTexture, NULL, PrimarySurface->pixels, PrimarySurface->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, sdlTexture, NULL, &stretchRect);
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
	int w, h;

	/* Initialize the SDL library */
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
		SDL_Log("Couldn't initialize SDL: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	 if (SDL_CreateWindowAndRenderer(0, 0, 0, &window, &renderer) < 0) {
        SDL_Log("Error creating window or renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_GetWindowSize(window, &w, &h);

	SDL_ShowCursor(SDL_DISABLE);

    sdlTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING,
        160, 160);
	PrimarySurface = SDL_CreateRGBSurface(0, 160, 160, 16, 0, 0, 0, 0);

    stretchRect.x = 0;
    stretchRect.y = 10;
    stretchRect.w = w;
    stretchRect.h = w;

    supervision_set_colour_scheme(COLOUR_SCHEME_DEFAULT); //COLOUR_SCHEME_DEFAULT COLOUR_SCHEME_GREEN COLOUR_SCHEME_AMBER COLOUR_SCHEME_BLUE
    supervision_init(); //Init the emulator
    supervision_turnSound(1);
//    supervision_reset();
	if(romname) loadROM(romname); else loadROM("rom.sv");
	supervision_load((u8*)buffer, (uint32)buffer_size);

    /* Main render loop */
    SDL_Log("main render loop");
     memset(screenbuffer, 0, sizeof(screenbuffer));
    short done = 0;
    short dirty = -1;
    SDL_Event event;
    do {
        controls_update();
        supervision_exec_fast((int16*)screenbuffer,1);
        Draw();
//        controls_reset();
    } while (!done && !paused);


    supervision_done(); //shutsdown the system
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
