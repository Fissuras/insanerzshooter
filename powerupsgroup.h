#ifndef powerupsgroup_h
#define powerupsgroup_h
	#include "global.h"
	#include "powerup.h"
	#include "il_player.h"
	#include <SDL/SDL_mixer.h>
	using namespace std;
	using std::vector;

	// Representa um conjunto de itens de powerups
	class PowerUpsGroup {

	public:

		vector<PowerUp> powerups; // vetor onde são armazenas os powerups
		IL_Player * player;

		// Construtor padrao
		PowerUpsGroup(IL_Player * playerNovo) {
		    player = playerNovo;
		}

		~PowerUpsGroup() {
		    powerups.clear();
		}

		// Adiciona um novo powerup ao group
		void add(PowerUp powerUp);

		// Executa o método act() de todos os powerups e os desenha na tela/surface
		void actAndDraw(SDL_Surface *tela);

		// Verifica se player pegou um powerup. Se sim, adiciona ao player a habilidade daquele powerup
		void checkCollision(Mix_Chunk * powerup, Mix_Chunk * doubleshoot, Mix_Chunk * tripleshoot, Mix_Chunk * insaneshoot, Mix_Chunk * speedup);

		double Abs(double number);

	};
#endif
