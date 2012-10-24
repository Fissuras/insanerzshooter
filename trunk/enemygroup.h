#ifndef enemygroup_h
#define enemygroup_h
	#include "global.h"
	#include "enemy.h"
	#include "particlegroup.h"
	#include "il_bullets.h"
	#include "il_player.h"
	#include <SDL/SDL_mixer.h>
	using namespace std;
	using std::vector;
	// Representa um conjunto de objetos da classe Enemy
	class EnemyGroup {

	public:

		vector<Enemy> enemies; // vetor onde s�o armazenas os enemies

		// Construtor padrao
		EnemyGroup() { }

		~EnemyGroup() {
		    enemies.clear();
		}

		// Adiciona um novo enemy ao group
		void add(Enemy sprite);

		// Desenha todos os enemies na tela/surface
		// Este m�todo normalmente n�o � usado
		void draw(SDL_Surface *tela);

		// Executa o m�todo act() de todos os enemies para que eles se movam, atirem, etc.
		// Este m�todo normalmente n�o � usado
		void act(SDL_Surface *tela);

		// Execura o m�todo act() de todos os enemies e os desenha na tela/surface
		void actAndDraw(SDL_Surface *tela, ParticleGroup *group);

		// Cria um novo enemy com type aleat�rio e posi��o X aleat�ria e o adiciona no vetor de enemies
		void createNewEnemy(IL_Sprite spriteEnemy, int type);

		// Verifica se algum dos enemies foi atingido por um bullet
		// @param bullets objeto do type IL_Bullets contendo todos os bullets que ser�o analisados
		void checkCollision(IL_Bullets *bullets, ParticleGroup *group, Mix_Chunk * explosion);

		// Esse metodo verifica colisao entre o player e um group de enemies.
		// N�o deveria estar aqui... num refactory ele deve mudar de lugar no futuro.
		bool checkCollision(IL_Player *player);

		// M�todo criado para calcular o valor absoluto (ou seja, sem o sinal) de um n�mero
		// @param numero n�mero a ser avaliado	
		double Abs(double number);

	};
#endif
