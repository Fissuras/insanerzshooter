#include "particula.h"
#include "global.h"

void Particula::draw(SDL_Surface *tela) {

	SDL_Rect posicaoSDL = SDL_Rect();

	if (type == 0) {
		posicaoSDL.h = 5;
		posicaoSDL.w = 5;
	} else if (type == 1) {
		posicaoSDL.h = 3;
		posicaoSDL.w = 3;
	} else if (type == 2) {
		posicaoSDL.h = 2;
		posicaoSDL.w = 2;
	} else if (type == 3) {
		posicaoSDL.h = 2;
		posicaoSDL.w = 2;
	}

	posicaoSDL.x = posicao.x;
	posicaoSDL.y = posicao.y;
	cor = SDL_MapRGB( tela->format, corParaFadeR, corParaFadeG, corParaFadeB);
	SDL_FillRect(tela, &posicaoSDL, cor);

	if (!GAME_PAUSED && (type == 0 || type == 1 || type == 3)) {
		if (corParaFadeR > 0) {
			corParaFadeR--;
		}
		if (corParaFadeG > 0) {
			corParaFadeG--;
		}
	}

}

void Particula::updatePosition() {

	if (!GAME_PAUSED) {
		if (type == 0) {
			posicao.x = posicao.x + (somaX / 3);
			posicao.y = posicao.y + (somaY / 3);
		} else if (type == 1) {
			posicao.y = posicao.y + somaY;
		} else if (type == 2) {
			posicao.y = posicao.y + somaY;
			if (posicao.y > SCREEN_HEIGHT) {
				posicao.y = 0;
				posicao.x = rand()%SCREEN_WIDTH;
			}
		} else if (type == 3) {
			posicao.y = posicao.y - somaY;
		}
	}

}

// Método criado para calcular o valor absoluto (ou seja, sem o sinal) de um número
// @param numero número a ser avaliado
double Particula::Abs(double number) {
    if ( number >= 0 ) {
        return number;
    } else {
        return -number;
    }
}
