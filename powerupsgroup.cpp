#include "powerupsgroup.h"

void PowerUpsGroup::add(PowerUp powerUp) {
	powerupsGroup.push_back(powerUp);
}

void PowerUpsGroup::actAndDraw(SDL_Surface *tela) {
	for (unsigned int i = 0; i < powerupsGroup.size(); i++) {
		powerupsGroup.at(i).act();
		if (powerupsGroup.at(i).position.y > 0) {
			SDL_Rect rect;
			rect.x = powerupsGroup.at(i).position.x;
			rect.y = powerupsGroup.at(i).position.y;

			SDL_BlitSurface(powerupsGroup.at(i).surface, NULL, tela, &rect);
		}
	}
}

void PowerUpsGroup::checkCollision(Mix_Chunk *powerup, Mix_Chunk *doubleshoot, Mix_Chunk *tripleshoot, Mix_Chunk *insaneshoot, Mix_Chunk *speedup) {
	for (unsigned int i = 0; i < powerupsGroup.size(); i++) {

		if ( Abs(player->playerSprite.position.x - powerupsGroup.at(i).position.x) < + 28 && Abs(player->playerSprite.position.y - powerupsGroup.at(i).position.y) < 28) {

			if (powerupsGroup.at(i).type == 0) { // bullet powerup

				player->bulletNumber = 100;
				if (player->gunSpeed > 230) {
					Mix_PlayChannel(-1, powerup, 0);
					player->gunSpeed = player->gunSpeed - 100;
				} else {

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

			} else { // speed powerup
				Mix_PlayChannel(-1, speedup, 0);
				if (player->speed < 4) {
					player->speed = player->speed + 0.5;
				}
			}

			powerupsGroup.erase(powerupsGroup.begin() + i);
		}

	}
}
