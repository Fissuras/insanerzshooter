#ifndef il_sprite_h
#define il_sprite_h

#ifdef ANDROID
	#include "SDL_image.h"
#else
	#include <SDL/SDL_image.h>
#endif
#include "global.h"
#include "il_position.h"

class IL_Sprite {

public:

	SDL_Surface *surface;
	IL_Position position;

	IL_Sprite() { }

	~IL_Sprite() { }

	IL_Sprite(char imageFilename[], int frameWidth) {

	    SDL_Surface* tempSurface = IMG_Load(imageFilename);
	    if (!tempSurface) {
	        fprintf(stderr, "Impossible to load %s %s:\n", imageFilename, IMG_GetError());
	        exit(-1);
	    } else {
	        // Optimize the image loaded
	        surface = SDL_DisplayFormat(tempSurface);
	        SDL_FreeSurface(tempSurface);
	    }

	    // Turn the PINK color, transparent
	    Uint32 colorkey = SDL_MapRGB(surface->format, 0xFF, 0, 0xFF);
	    SDL_SetColorKey(surface, SDL_SRCCOLORKEY, colorkey);

	    frameWidth = frameWidth;//Not used

	    position.x = 0;
	    position.y = 0;
	}

	IL_Sprite(SDL_Surface *newSurface, int frameWidth) {

	    surface = newSurface;

	    // Turn the PINK color, transparent
	    Uint32 colorkey = SDL_MapRGB( surface->format, 0xFF, 0, 0xFF);
	    SDL_SetColorKey( surface, SDL_SRCCOLORKEY, colorkey);

	    frameWidth = frameWidth;//Not used

	    position.x = 0;
	    position.y = 0;
	}

	virtual void act();

private:

	int frameWidth;

};

#endif
