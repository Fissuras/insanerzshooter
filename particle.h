#ifndef particle_h
#define particle_h

#include "global.h"
#include "il_position.h"
#ifdef ANDROID
	#include "SDL_image.h"
#else
	#include <SDL/SDL_image.h>
#endif

class Particle {

public:

	IL_Position position;
	SDL_Rect positionSDL;
	float sumX;
	float sumY;
	int redColorFade;
	int greenColorFade;
	int blueColorFade;
	Uint32 color;
	int type;

	Particle() {
	    redColorFade = 0;
	    greenColorFade = 0;
	    blueColorFade = 0;
	    SDL_Rect positionSDL = SDL_Rect();
	}

	Particle(int x, int y, int newType) {

	    type = newType;
	    position.x = x;
	    position.y = y;
	    sumX = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
	    sumY = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
	    if (Abs(sumX + sumY) < 0.01) {
	        sumX = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
	        sumY = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
	    }

	    if ( (type == 1 || type==2 || type == 3) && sumY < 0) {
	        sumY = -sumY;
	    }

	    if (type == 0) {
	        redColorFade = 120 + (rand()%135);
	        greenColorFade = 0;
	        blueColorFade = 0;
	    } else if (type == 1) {
	        redColorFade = 229 + (rand()%21);
	        greenColorFade = 148 + (rand()%40);
	        blueColorFade = 6;
	    } else if (type == 2) {
	        redColorFade = 255;
	        greenColorFade = 255;
	        blueColorFade = 255;
	    } else if (type == 3) {
	        redColorFade = 0 + (rand()%102);
	        greenColorFade = 150 + (rand()%45);
	        blueColorFade = 185 + (rand()%70);
	    }

	}

	~Particle() {}

	void draw(SDL_Surface *tela);

	void updatePosition();

};

#endif
