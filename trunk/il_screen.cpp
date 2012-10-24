#include "il_screen.h"

void IL_Screen::draw(IL_Sprite &sprite) {
	SDL_Rect rect;
	rect.x = sprite.position.x;
	rect.y = sprite.position.y;
	SDL_BlitSurface(sprite.surface, NULL, surface, &rect);
}

void IL_Screen::atualizar() {
	fps->start();
	frame++;
	if ( fps->get_ticks() < 1000 / FRAMES_PER_SECOND ) {
		//Sleep the remaining frame time
		SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps->get_ticks());
	}
}

void IL_Screen::limpar() {
	SDL_FillRect(surface, NULL, 0);
}

void IL_Screen::drawHeader() {
	SDL_Rect posicao = SDL_Rect();
	posicao.x = 0;
	posicao.y = 0;

	#ifdef psp
	posicao.h = 30;
	#else
	posicao.h = 40;
	#endif

	posicao.w = SCREEN_WIDTH;
	SDL_FillRect(surface, &posicao, corAzul);

	#ifdef psp
	posicao.y = 30;
	#else
	posicao.y = 40;
	#endif

	posicao.h = 1;
	posicao.w = SCREEN_WIDTH;
	SDL_FillRect(surface, &posicao, corBranca);
}

void IL_Screen::drawBarraFirePower(IL_Player *player) {
	SDL_Rect posicao = SDL_Rect();
	if (PLAYER_ALIVE) {
		// desenhando a barra do fire power
		posicao.x = SCREEN_WIDTH * 0.4;
		posicao.y = 30;
		posicao.h = 4;
		posicao.w = player->bulletNumber;
		if (player->bulletNumber <= 20) {
			SDL_FillRect(surface, &posicao, corVermelha);
		} else if (player->bulletNumber <= 80) {
			SDL_FillRect(surface, &posicao, corAmarela);
		} else {
			SDL_FillRect(surface, &posicao, corVerde);
		}
	}
}
