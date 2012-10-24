#ifndef il_screen_h
#define il_screen_h
	#include "global.h"
	#include <SDL/SDL_image.h>
	#include "timer.h"
	#include "il_sprite.h"
	#include "il_player.h"
	// Classe responvável por exibir as coisas na tela.
	class IL_Screen {

	public:

		// surface que será exibida ao usuário
		SDL_Surface *surface;

		// contador de frames
		int frame;

		Uint32 corBranca;
		Uint32 corAmarela;
		Uint32 corVerde;
		Uint32 corVermelha;
		Uint32 corAzul;
		bool estaEmFullScreen;

		// Timer controlador do frames por segundo
		Timer *fps;

		IL_Screen() {
		    estaEmFullScreen = false;
		}

		~IL_Screen() {
		    delete fps;
		    fps = NULL;
		}

		// Construtor
		// @param telaCheia caso true, o jogo será executado em fullscreen.
		// @param resolucaoWidth da tela (padrao = 800)
		// @param resolucaoHeight da tela (padrao = 600)
		// @param bits de resolução (padrao = 32)
		IL_Screen(bool telaCheia, int resolucaoWidth = SCREEN_WIDTH, int resolucaoHeight = SCREEN_HEIGHT, int bits = 32) {

		    frame = 0;
		    fps = new Timer();
		    fps->start();

		    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
		        fprintf(stderr,"Não foi possível inicializar a SDL: %s\n", SDL_GetError());
		        exit(-1);
		    }

		    /* Setando resolução */
		    if (telaCheia == true) {
		        surface = SDL_SetVideoMode(resolucaoWidth, resolucaoHeight, bits, SDL_DOUBLEBUF | SDL_FULLSCREEN);
		    } else {
		        #ifdef psp
		        surface = SDL_SetVideoMode(resolucaoWidth, resolucaoHeight, bits, SDL_SWSURFACE);
		        #else
		        surface = SDL_SetVideoMode(resolucaoWidth, resolucaoHeight, bits, SDL_DOUBLEBUF | SDL_HWSURFACE);
		        #endif
		    }

		    if ( !surface ) {
		        fprintf(stderr, "Não foi possível inicializar modo de vídeo %s\n", SDL_GetError());
		        exit(-1);
		    }

		    corBranca = SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF);
		    corAmarela = SDL_MapRGB( surface->format, 0xFF, 0xFF, 0);
		    corVerde = SDL_MapRGB( surface->format, 0, 0xFF, 0);
		    corVermelha = SDL_MapRGB( surface->format, 0xFF, 0, 0);
		    corAzul = SDL_MapRGB( surface->format, 0, 0x22, 0xFF);
		    estaEmFullScreen = telaCheia;

		}

		void draw(IL_Sprite &sprite);

		void atualizar();

		void limpar();

		void drawHeader();

		void drawBarraFirePower(IL_Player *player);

	};
#endif
