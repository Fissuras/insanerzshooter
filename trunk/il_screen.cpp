#include "il_screen.h"

void IL_Screen::draw(IL_Sprite &sprite) {
	SDL_Rect rect;
	rect.x = sprite.position.x;
	rect.y = sprite.position.y;
	SDL_BlitSurface(sprite.surface, NULL, surface, &rect);
}

void IL_Screen::clean() {
	SDL_FillRect(surface, NULL, 0);
}

void IL_Screen::drawHeader() {
	SDL_Rect headerPosition = SDL_Rect();
	headerPosition.x = 0;
	headerPosition.y = 0;

	headerPosition.h = NORMAL_FONT_SIZE * 2;
	headerPosition.y = NORMAL_FONT_SIZE * 2;

	headerPosition.w = SCREEN_WIDTH;
	SDL_FillRect(surface, &headerPosition, blueColor);

	headerPosition.h = 1;
	headerPosition.w = SCREEN_WIDTH;
	SDL_FillRect(surface, &headerPosition, whiteColor);
}

void IL_Screen::drawFirePowerBar(IL_Player *player) {
	SDL_Rect firepowerbarPosition = SDL_Rect();
	if (PLAYER_ALIVE) {
		firepowerbarPosition.x = (SCREEN_WIDTH / 4) * 2;
		firepowerbarPosition.y = 30;
		firepowerbarPosition.h = 4;
		firepowerbarPosition.w = player->bulletNumber;
		if (player->bulletNumber <= 20) {
			SDL_FillRect(surface, &firepowerbarPosition, redColor);
		} else if (player->bulletNumber <= 80) {
			SDL_FillRect(surface, &firepowerbarPosition, yellowColor);
		} else {
			SDL_FillRect(surface, &firepowerbarPosition, greenColor);
		}
	}
}
