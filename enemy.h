#ifndef enemy_h
#define enemy_h
	#include "global.h"
	#include "il_sprite.h"

	// Representa uma nave inimiga. Nada mais é do que um Sprite com o método act implementado e alguns novos atributos.
	class Enemy : public IL_Sprite {

	public:

		float speed;
		float seno; // usado para fazer movimentações loucas nas naves hehe (seria bom colocar um nome melhor pra essa var)
		int type;

		// Construtor padrao
		Enemy() {
		    position.y = -250; // posiciona o enemy pouco acima do topo da tela
		    speed = rand()%100; // numero aleatorio entre 0 e 100
		    speed = (speed / 100) + 1; // speed será aleatória entre 1 e 2
		    seno = (rand()%100 / 100) - 0.5; // iniciando seno com um valor aleatorio entre -0.5 e +0.5
		    type = 0; // indica o type de nave. Depdendo do type ela vai ter um comportamento diferente no método act()
		}

		~Enemy() {}

		// Construtor
		// Para evitar ficar criando um monte de imagens, deve-se carregar a imagem antes de criar um enemy e passar apenas uma referencia para a surface
		// O type indica qual será o comportamento do enemy no método act()
		Enemy(IL_Sprite novoSprite, int typeNovo) {
		    type = typeNovo;
		    surface = novoSprite.surface;
		    position.x = rand() % int((SCREEN_WIDTH - (SCREEN_WIDTH * 0.087)));
		    position.y = -250;
		    speed = rand()%100;
		    speed = (speed / 100) + 1; // speed será aleatória entre 1 e 2
		    seno = (rand()%100 / 100) - 0.5; // iniciando seno com um valor aleatorio entre -0.5 e +0.5
		}

		// Método responsável pelo comportamento do enemy.
		// Cada type de enemy tem um comportamento.
		// Talvez fosse mais interessante usar polimorfismo e dividir em várias classes os enemies, mas tive problemas para fazer isso em C++.
		void act();

	};
#endif
