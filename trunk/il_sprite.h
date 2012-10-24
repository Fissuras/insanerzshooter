#ifndef il_sprite_h
#define il_sprite_h
	#include <SDL/SDL_image.h>
	#include "global.h"
	#include "il_position.h"

	// Representa um Sprite; ou seja, uma imagem/surface com posição X,Y e que no futuro suportará vários frames (animação)
	class IL_Sprite {

	public:

		// Quando utilizamos SDL, armazenamos todas as imagens em uma Surface.
		SDL_Surface *surface;
		// Representa a posição X,Y onde o Sprite se encontra na tela.
		IL_Position position;

		// Construtor padrao
		IL_Sprite() { }

		~IL_Sprite() { }

		// Construtor
		IL_Sprite(char imageFilename[], int frameWidth) {

		    // Carregando a imagem do sprite e colocando ela em um Surface
		    SDL_Surface* tempSurface = IMG_Load(imageFilename);
		    if (!tempSurface) {
		        fprintf(stderr, "Impossible to load %s %s:\n", imageFilename, IMG_GetError());
		        exit(-1);
		    } else {
		        // Este comando abaixo serve para otimizar a imagem
		        surface = SDL_DisplayFormat( tempSurface);
		        SDL_FreeSurface( tempSurface);
		    }

		    // Em todas as imagens a cor 255,0,255 ficará transparente
		    Uint32 colorkey = SDL_MapRGB( surface->format, 0xFF, 0, 0xFF);
		    SDL_SetColorKey( surface, SDL_SRCCOLORKEY, colorkey);

		    // Não usado por enquanto... mais pra frente teremos animações e essa variavel será usada.
		    frameWidth = frameWidth;

		    // Iniciando o Sprite na posição 0,0
		    position.x = 0;
		    position.y = 0;
		}

		// Construtor onde você passa direto um ponteiro de uma surface.
		// É recomenado utiliza-lo quando você vai utilizar uma mesma imagem para varios sprites, evitando assim carregar uma mesma imagem várias vezes
		IL_Sprite(SDL_Surface *newSurface, int frameWidth) {

		    surface = newSurface;

		    // Colocando transparencia para a cor de fundo 255,0,255
		    Uint32 colorkey = SDL_MapRGB( surface->format, 0xFF, 0, 0xFF);
		    SDL_SetColorKey( surface, SDL_SRCCOLORKEY, colorkey);

		    // Não é utilizado no momento, será usado no futuro para fazer animações
		    frameWidth = frameWidth;

		    // Posicionando o sprite em 0,0,
		    position.x = 0;
		    position.y = 0;
		}

		// Método act será sobreescrito por outras classes.
		// Representa a ação tomada pelos sprites, como sua movimentação e seu comportamento em geral.
		virtual void act();

	private:

		// No futuro, cada imagem/surface poderá conter vários frames para realizar animações.
		int frameWidth;

	};
#endif
