#ifndef particula_h
#define particula_h
	#include "il_position.h"
	#include <SDL/SDL_image.h>
	// Particula
	class Particula {

	public:

		IL_Position posicao;
		SDL_Rect posicaoSDL;
		float somaX;
		float somaY;
		int corParaFadeR;
		int corParaFadeG;
		int corParaFadeB;
		Uint32 cor;
		int type;

		Particula() {
		    corParaFadeR = 0;
		    corParaFadeG = 0;
		    corParaFadeB = 0;
		    SDL_Rect posicaoSDL = SDL_Rect();
		}

		Particula(int x, int y, int typeNovo) {

		    type = typeNovo;
		    posicao.x = x;
		    posicao.y = y;
		    somaX = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
		    somaY = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
		    if (Abs(somaX + somaY) < 0.01) {
		        somaX = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
		        somaY = ((float(rand()) / RAND_MAX) - 0.5) * (rand()%6 + 2);
		    }

		    if ( (type == 1 || type==2 || type == 3) && somaY < 0) {
		        somaY = -somaY;
		    }

		    if (type == 0) {
		        corParaFadeR = 120 + (rand()%135);
		        corParaFadeG = 0;
		        corParaFadeB = 0;
		    } else if (type == 1) {
		        corParaFadeR = 229 + (rand()%21);
		        corParaFadeG = 148 + (rand()%40);
		        corParaFadeB = 6;
		    } else if (type == 2) {
		        corParaFadeR = 255;
		        corParaFadeG = 255;
		        corParaFadeB = 255;
		    } else if (type == 3) {
		        corParaFadeR = 0 + (rand()%102);
		        corParaFadeG = 150 + (rand()%45);
		        corParaFadeB = 185 + (rand()%70);
		    }

		}

		~Particula() {}

		// Método criado para calcular o valor absoluto (ou seja, sem o sinal) de um número
		// @param numero número a ser avaliado
		double Abs(double number);

		void draw(SDL_Surface *tela);

		void updatePosition();

	};
#endif
