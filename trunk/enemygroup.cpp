#include "enemygroup.h"

// Adiciona um novo enemy ao group
void EnemyGroup::add(Enemy sprite) {
	enemies.push_back(sprite);
}

// Desenha todos os enemies na tela/surface
// Este m�todo normalmente n�o � usado
void EnemyGroup::draw(SDL_Surface *tela) {
	for (unsigned int i = 0; i < enemies.size(); i++) {
		SDL_Rect rect;
		rect.x = enemies.at(i).position.x;
		rect.y = enemies.at(i).position.y;
		SDL_BlitSurface(enemies.at(i).surface, NULL, tela, &rect);
	}
}

// Executa o m�todo act() de todos os enemies para que eles se movam, atirem, etc.
// Este m�todo normalmente n�o � usado
void EnemyGroup::act(SDL_Surface *tela) {
	if (!GAME_PAUSED) {
		for (unsigned int i = 0; i < enemies.size(); i++) {
			enemies.at(i).act();
		}
	}
}

// Execura o m�todo act() de todos os enemies e os desenha na tela/surface
void EnemyGroup::actAndDraw(SDL_Surface *tela, ParticleGroup *group) {
	for (unsigned int i = 0; i < enemies.size(); i++) {
		if (!GAME_PAUSED) {
			enemies.at(i).act();
		}
		if (enemies.at(i).position.y > 0) {
			SDL_Rect rect;
			rect.x = enemies.at(i).position.x;
			rect.y = enemies.at(i).position.y;
			SDL_BlitSurface(enemies.at(i).surface, NULL, tela, &rect);
		}
		int probDeCriarParticulaDeFogo = 10 - ((int) enemies.at(i).speed); // a prob de criar particula de fogo ser� proporcional a vel da nave
		if (!GAME_PAUSED) {
			if (rand()%probDeCriarParticulaDeFogo == 1) {
				group->addParticleSystem(enemies.at(i).position.x, enemies.at(i).position.y, 3, 0, 20);
			}
		}
	}
}

// Cria um novo enemy com type aleat�rio e posi��o X aleat�ria e o adiciona no vetor de enemies
void EnemyGroup::createNewEnemy(IL_Sprite spriteEnemy, int type) {
	spriteEnemy.position.x = rand() % SCREEN_WIDTH;
	spriteEnemy.position.y= -250;
	Enemy enemy = Enemy(spriteEnemy, type);
	add(enemy);
}

// Verifica se algum dos enemies foi atingido por um bullet
// @param bullets objeto do type IL_Bullets contendo todos os bullets que ser�o analisados
void EnemyGroup::checkCollision(IL_Bullets *bullets, ParticleGroup *group, Mix_Chunk * explosion) {
	// para cada enemy
	for (unsigned int i = 0; i < enemies.size(); i++) {

		// verifica colisao com cada bullet
		for (unsigned int j = 0; j < bullets->bullets.size(); j++) {

			// Para tornar o m�todo mais r�pido � feita uma verifica��o simples de colis�o:
			// se os objetos estiverem a um raio menos do que 28, � por que houve colis�o
			if ( Abs(bullets->bullets.at(j).position.x - enemies.at(i).position.x) < + 28 && Abs(bullets->bullets.at(j).position.y - enemies.at(i).position.y) < 28) {
				group->addParticleSystem(bullets->bullets.at(j).position.x, bullets->bullets.at(j).position.y, 0, NUMBER_OF_PARTICLES_ENEMY_EXPLOSION, 200);
				if (PLAYER_SCORE < 99999) { // atualiza contador de playerScore
					PLAYER_SCORE++;
				}
				enemies.erase(enemies.begin() + i);
				bullets->bullets.erase(bullets->bullets.begin() + j);
				Mix_PlayChannel(-1, explosion, 0);
				break;
			}
		}

	}
}

// Esse metodo verifica colisao entre o player e um group de enemies.
// N�o deveria estar aqui... num refactory ele deve mudar de lugar no futuro.
bool EnemyGroup::checkCollision(IL_Player *player) {
	for (unsigned int i=0; i < enemies.size(); i++) {
		if (Abs(player->playerSprite.position.x - enemies.at(i).position.x) < 20 && Abs(player->playerSprite.position.y - enemies.at(i).position.y) < 20) {
			return true;
		}
	}

	return false;
}

// M�todo criado para calcular o valor absoluto (ou seja, sem o sinal) de um n�mero
// @param numero n�mero a ser avaliado
double EnemyGroup::Abs(double number) {
    if ( number >= 0 ) {
        return number;
    } else {
        return -number;
    }
}
