#include "particle.h"
#include "global.h"

void Particle::draw(SDL_Surface *tela) {

	SDL_Rect positionSDL = SDL_Rect();

	if (type == 0) {
		positionSDL.h = 5;
		positionSDL.w = 5;
	} else if (type == 1) {
		positionSDL.h = 3;
		positionSDL.w = 3;
	} else if (type == 2) {
		positionSDL.h = 2;
		positionSDL.w = 2;
	} else if (type == 3) {
		positionSDL.h = 2;
		positionSDL.w = 2;
	}

	positionSDL.x = position.x;
	positionSDL.y = position.y;
	color = SDL_MapRGB( tela->format, redColorFade, greenColorFade, blueColorFade);
	SDL_FillRect(tela, &positionSDL, color);

	if (!GAME_PAUSED && (type == 0 || type == 1 || type == 3)) {
		if (redColorFade > 0) {
			redColorFade--;
		}
		if (greenColorFade > 0) {
			greenColorFade--;
		}
	}

}

void Particle::updatePosition() {

	if (!GAME_PAUSED) {
		if (type == 0) {
			position.x = position.x + (sumX / 3);
			position.y = position.y + (sumY / 3);
		} else if (type == 1) {
			position.y = position.y + sumY;
		} else if (type == 2) {
			position.y = position.y + sumY;
			if (position.y > SCREEN_HEIGHT) {
				position.y = 0;
				position.x = rand()%SCREEN_WIDTH;
			}
		} else if (type == 3) {
			position.y = position.y - sumY;
		}
	}

}
