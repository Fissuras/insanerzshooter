#include "bullet.h"

void Bullet::draw(SDL_Surface *tela) {

	SDL_Rect tiroRect;
	tiroRect.x = position.x;
	tiroRect.y = position.y;
	tiroRect.w = width;
	tiroRect.h = height;
	corTiroVelLenta = SDL_MapRGB( tela->format, 0x81, 0xFF, 0x1B);
	corTiroVelMedia = SDL_MapRGB( tela->format, 0xD4, 0xC4, 0x39);
	corTiroVelRapida = SDL_MapRGB( tela->format, 0xD1, 0x1B, 0xFF);
	if (gunSpeed == 2) {
		SDL_FillRect(tela, &tiroRect, corTiroVelRapida);
	} else if (gunSpeed == 1) {
		SDL_FillRect(tela, &tiroRect, corTiroVelMedia);
	} else {
		SDL_FillRect(tela, &tiroRect, corTiroVelLenta);
	}

}

void Bullet::updatePosition() {
	if (!GAME_PAUSED) {
		// forward bullet
		if (type ==0) {
			position.y = position.y - 5;
			// left forward bullet
		} else if (type ==1) {
			position.x = position.x - 1;
			position.y = position.y - 5;
			// right forward bullet
		} else if (type ==2) {
			position.x = position.x + 1;
			position.y = position.y - 5;
		}
	}
}
