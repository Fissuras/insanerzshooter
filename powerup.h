#ifndef powerup_h
#define powerup_h
	#include "global.h"
	#include "il_sprite.h"
	class PowerUp : public IL_Sprite {

	public:

		float speed;
		int type;

		// Construtor padrao
		PowerUp() {
		    position.y = -250; // posiciona o powerup acima do topo da tela
		    speed = rand()%100; // numero aleatorio entre 0 e 100
		    speed = (speed / 100) + 1; // speed será aleatória entre 1 e 2
		    type = 0; // indica o type de powerup. Dependendo do type vai ter um comportamento diferente no método act()
		}

		~PowerUp() {}

		// Construtor
		// Para evitar ficar criando um monte de imagens, deve-se carregar a imagem antes de criar um powerup e passar apenas uma referencia para a surface
		// O type indica qual será o comportamento do powerup no método act()
		PowerUp(IL_Sprite novoSprite, int typeNovo) {
		    type = typeNovo;
		    surface = novoSprite.surface;
		    position.x = rand() % int(SCREEN_WIDTH - (SCREEN_WIDTH * 0.125));
		    position.y = -250;
		    speed = rand()%100;
		    speed = (speed / 100) + 1; // speed será aleatória entre 1 e 2
		}

		void act();

	};
#endif
