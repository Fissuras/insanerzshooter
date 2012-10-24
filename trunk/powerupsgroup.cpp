#include "powerupsgroup.h"

// Adiciona um novo powerup ao group
void PowerUpsGroup::add(PowerUp powerUp) {
	powerups.push_back(powerUp);
}

// Executa o método act() de todos os powerups e os desenha na tela/surface
void PowerUpsGroup::actAndDraw(SDL_Surface *tela) {
	for (unsigned int i = 0; i < powerups.size(); i++) {
		powerups.at(i).act();
		if (powerups.at(i).position.y > 0) {
			SDL_Rect rect;
			rect.x = powerups.at(i).position.x;
			rect.y = powerups.at(i).position.y;

			SDL_BlitSurface(powerups.at(i).surface, NULL, tela, &rect);
		}
	}
}

// Verifica se player pegou um powerup. Se sim, adiciona ao player a habilidade daquele powerup
void PowerUpsGroup::checkCollision(Mix_Chunk * powerup, Mix_Chunk * doubleshoot, Mix_Chunk * tripleshoot, Mix_Chunk * insaneshoot, Mix_Chunk * speedup) {
	// para cada powerup
	for (unsigned int i = 0; i < powerups.size(); i++) {

		// verifica colisao com player
		if ( Abs(player->playerSprite.position.x - powerups.at(i).position.x) < + 28 && Abs(player->playerSprite.position.y - powerups.at(i).position.y) < 28) {

			if (powerups.at(i).type == 0) { // player pegou um powerup para o bullet

				player->bulletNumber = 100;
				if (player->gunSpeed > 230) {
					Mix_PlayChannel(-1, powerup, 0);
					player->gunSpeed = player->gunSpeed - 100;
				} else {

					// tocando respectivo som
					switch (player->gun) {
					case 0:
						Mix_PlayChannel(-1, doubleshoot, 0);
						break;
					case 1:
						Mix_PlayChannel(-1, tripleshoot, 0);
						break;
					case 2:
						Mix_PlayChannel(-1, insaneshoot, 0);
						break;
					case 3:
						Mix_PlayChannel(-1, powerup, 0);
						break;
					default:
						break;
					}

					if (player->gun < 3) {
						player->gun = player->gun + 1;
						player->gunSpeed = 430;
					}
				}

			} else { // player pegou um powerup para speed
				Mix_PlayChannel(-1, speedup, 0);
				if (player->speed < 4) {
					player->speed = player->speed + 0.5;
				}
			}

			powerups.erase(powerups.begin() + i);
		}

	}
}

// Método criado para calcular o valor absoluto (ou seja, sem o sinal) de um número
// @param numero número a ser avaliado
double PowerUpsGroup::Abs(double number) {
    if ( number >= 0 ) {
        return number;
    } else {
        return -number;
    }
}
