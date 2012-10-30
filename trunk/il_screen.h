#ifndef il_screen_h
#define il_screen_h

#include "global.h"
#ifdef ANDROID
	#include "SDL_image.h"
#else
	#include <SDL/SDL_image.h>
#endif
#include "timer.h"
#include "il_sprite.h"
#include "il_player.h"

class IL_Screen {

public:

	SDL_Surface *surface;
	int frame;
	Uint32 whiteColor;
	Uint32 yellowColor;
	Uint32 greenColor;
	Uint32 redColor;
	Uint32 blueColor;

	Timer *fps;

	IL_Screen() {
		frame = 0;
	    fps = new Timer();
	    fps->start();

	    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
	        fprintf(stderr,"Impossible to initialize SDL: %s\n", SDL_GetError());
	        exit(-1);
	    }

        #ifdef PSP
        	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE);
		#elif ANDROID
        	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_SWSURFACE|SDL_FULLSCREEN);
        #else
        	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_DOUBLEBUF | SDL_HWSURFACE);
        #endif

	    if ( !surface ) {
	        fprintf(stderr, "Impossible to initialize video: %s\n", SDL_GetError());
	        exit(-1);
	    }

	    whiteColor = SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF);
	    yellowColor = SDL_MapRGB( surface->format, 0xFF, 0xFF, 0);
	    greenColor = SDL_MapRGB( surface->format, 0, 0xFF, 0);
	    redColor = SDL_MapRGB( surface->format, 0xFF, 0, 0);
	    blueColor = SDL_MapRGB( surface->format, 0, 0x22, 0xFF);
	}

	~IL_Screen() {
	    delete fps;
	    fps = NULL;
	}

	void draw(IL_Sprite &sprite);

	void clean();

	void drawHeader();

	void drawFirePowerBar(IL_Player *player);

};

#endif
