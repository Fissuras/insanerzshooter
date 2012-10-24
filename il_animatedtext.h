#ifndef il_animatedtext_h
#define il_animatedtext_h
	#include "global.h"
	#include "il_position.h"
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_ttf.h>
	#include <math.h>

	class IL_AnimatedText {

	public:

		IL_Position posicaoChar;
		bool estaSubindo;
		SDL_Surface *surface;
		float varAuxiliarCos;

		IL_AnimatedText () {
			estaSubindo = false;
			posicaoChar.x = 0;
			posicaoChar.y = 0;
			surface = NULL;
			varAuxiliarCos = 0;
		}

		~IL_AnimatedText () {}

		IL_AnimatedText(char caracter[], float newX, float newY, bool estaSubindoNovo, TTF_Font *font) {
			estaSubindo = estaSubindoNovo;
			posicaoChar.x = newX;
			posicaoChar.y = newY;
			SDL_Color textColor = { 255, 255, 255 };
			surface = TTF_RenderText_Solid( font, caracter, textColor);
			varAuxiliarCos = 0;
		}

		void animate();

	};
#endif
