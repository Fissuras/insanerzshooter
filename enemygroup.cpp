#include "enemygroup.h"

void EnemyGroup::add(Enemy sprite) {
	enemies.push_back(sprite);
}

void EnemyGroup::draw(SDL_Surface *tela) {
	for (unsigned int i = 0; i < enemies.size(); i++) {
		SDL_Rect rect;
		rect.x = enemies.at(i).position.x;
		rect.y = enemies.at(i).position.y;
		SDL_BlitSurface(enemies.at(i).surface, NULL, tela, &rect);
	}
}

void EnemyGroup::act(SDL_Surface *tela) {
	if (!GAME_PAUSED) {
		for (unsigned int i = 0; i < enemies.size(); i++) {
			enemies.at(i).act();
		}
	}
}

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
		int probDeCriarParticulaDeFogo = 10 - ((int) enemies.at(i).speed); //fire particle is proportional within enemy speed
		if (!GAME_PAUSED) {
			if (rand()%probDeCriarParticulaDeFogo == 1) {
				group->addParticleSystem(enemies.at(i).position.x, enemies.at(i).position.y, 3, 0, 20);
			}
		}
	}
}

// Create a new enemy with random type and position X
void EnemyGroup::createNewEnemy(IL_Sprite spriteEnemy, int type) {
	spriteEnemy.position.x = rand() % SCREEN_WIDTH;
	spriteEnemy.position.y= -250;
	Enemy enemy = Enemy(spriteEnemy, type);
	add(enemy);
}

// Check the collision with bullet
void EnemyGroup::checkCollision(IL_Bullets *bullets, ParticleGroup *group, Mix_Chunk *explosion) {
	for (unsigned int i = 0; i < enemies.size(); i++) {
		for (unsigned int j = 0; j < bullets->bullets.size(); j++) {
			if (Abs(bullets->bullets.at(j).position.x - enemies.at(i).position.x) < enemies.at(i).surface->w && 
				Abs(bullets->bullets.at(j).position.y - enemies.at(i).position.y) < enemies.at(i).surface->h) {
				group->addParticleSystem(bullets->bullets.at(j).position.x, bullets->bullets.at(j).position.y, 0, NUMBER_OF_PARTICLES_ENEMY_EXPLOSION, 200);
				if (PLAYER_SCORE < 99999) {
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

// Check the collision with player
bool EnemyGroup::checkCollision(IL_Player *player) {
	for (unsigned int i=0; i < enemies.size(); i++) {
		if (Abs(player->playerSprite.position.x - enemies.at(i).position.x) < enemies.at(i).surface->w &&
			Abs(player->playerSprite.position.y - enemies.at(i).position.y) < enemies.at(i).surface->h) {
			return true;
		}
	}

	return false;
}
