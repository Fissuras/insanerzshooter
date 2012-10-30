#include "il_inputhandle.h"

//Only check if any button pressed
void IL_InputHandle::checkPressed() {

	if ( SDL_PollEvent(&event) != 0 ) {

		switch (event.type) {
			case SDL_QUIT:
				exit(0); //Close the app
				break;
			default:
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					leftPressed = true;
					break;
				case SDLK_RIGHT:
					rightPressed = true;
					break;
				case SDLK_DOWN:
					downPressed = true;
					break;
				case SDLK_UP:
					upPressed = true;
					break;
				case SDLK_ESCAPE:
					exit(0);
					break;
				case SDLK_p:
					if (PLAYER_ALIVE) {
						pausePressed = true;
					}
					break;
				case SDLK_PAUSE:
					if (PLAYER_ALIVE) {
						pausePressed = true;
					}
					break;
				case SDLK_SPACE:
					shotButtonPressed = true;
					break;
				default:
					break;
				}
			break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					leftPressed = false;
					break;
				case SDLK_RIGHT:
					rightPressed = false;
					break;
				case SDLK_DOWN:
					downPressed = false;
					break;
				case SDLK_UP:
					upPressed = false;
					break;
				case SDLK_SPACE:
					shotButtonPressed = false;
					break;
				case SDLK_p:
					pausePressed = false;
					break;
				case SDLK_PAUSE:
					pausePressed = false;
					break;
				default:
					break;
				}
			break;

			#ifdef PSP
			case SDL_JOYBUTTONDOWN:
				switch (event.jbutton.button) {
				case 7:  //LEFT
					leftPressed = true;
					break;
				case 9: // RIGHT
					rightPressed = true;
					break;
				case 6: // DOWN
					downPressed = true;
					break;
				case 8: // UP
					upPressed = true;
					break;
				case 11: // start
					if (PLAYER_ALIVE) {
						pausePressed = true;
					}
					break;
				case 2: // X
					shotButtonPressed = true;
					break;
				default:
					break;
				}
			break;

			case SDL_JOYBUTTONUP:
				switch (event.jbutton.button) {
				case 7:  //LEFT
					leftPressed = false;
					break;
				case 9: // RIGHT
					rightPressed = false;
					break;
				case 6: // DOWN
					downPressed = false;
					break;
				case 8: // UP
					upPressed = false;
					break;
				case 11: // start
					pausePressed = false;
					break;
				case 2: // X
					shotButtonPressed = false;
					break;
				default:
					break;
				}
			break;
			#endif
		}
	}
}

//Execute the actions
void IL_InputHandle::handleActions() {
	if (pausePressed == true) {
		GAME_PAUSED = !GAME_PAUSED;
		pausePressed = false;
	}
	if (!GAME_PAUSED && leftPressed == true && player->playerSprite.position.x > 10) {
		player->playerSprite.position.x = player->playerSprite.position.x - player->speed;
	}
	if (!GAME_PAUSED && rightPressed == true && player->playerSprite.position.x < SCREEN_WIDTH - 20) {
		player->playerSprite.position.x = player->playerSprite.position.x + player->speed;
	}
	if (!GAME_PAUSED && upPressed == true && player->playerSprite.position.y > int((SCREEN_HEIGHT * 0.25)) ) {
		player->playerSprite.position.y = player->playerSprite.position.y - player->speed;
	}
	if (!GAME_PAUSED && downPressed == true && player->playerSprite.position.y < SCREEN_HEIGHT - 28) {
		player->playerSprite.position.y = player->playerSprite.position.y + player->speed;
	}
	if (!GAME_PAUSED && shotButtonPressed == true) {

		if (PLAYER_ALIVE == false) {
			if (player->deathTimer->get_ticks() > 5000) {
				//restart the game
				PLAYER_ALIVE = true;
				PLAYER_LIVES = 2;
				player->restart();
				enemiesGroup->enemies.clear();
				powerupsGroup->powerupsGroup.clear();
				shotButtonPressed = false; //avoid bug shot when restart the game
			}
		} else {
			// check if can shot
			if (shotTimer->get_ticks() > player->gunSpeed) {
				Mix_PlayChannel(-1, laser, 0);
				shotTimer->start();
				// decrease the bullet number (except the first gun)
				if (player->gun != 0 || player->gunSpeed < 430) {
					player->bulletNumber--;
				}

				int gunSpeed = 0;
				if (player->gunSpeed == 330) {
					gunSpeed = 1;
				} else if (player->gunSpeed == 230) {
					gunSpeed = 2;
				}

				// If more than 300 points, six bullet gun XD
				int posX = player->playerSprite.position.x;
				int posY = player->playerSprite.position.y;
				if (player->gun == 3) {
					bullets->newShot(gunSpeed, posX + 42, posY - 10, 2);
					bullets->newShot(gunSpeed, posX + 32, posY - 10, 2);
					bullets->newShot(gunSpeed, posX - 20, posY - 10, 1);
					bullets->newShot(gunSpeed, posX - 10, posY - 10, 1);
					bullets->newShot(gunSpeed, posX + 3, posY - 15, 0);
					bullets->newShot(gunSpeed, posX + 23, posY - 15, 0);
					// if more than 200 points, triple bullet gun :D
				} else if (player->gun == 2) {
					bullets->newShot(gunSpeed, posX + 32, posY - 10, 2);
					bullets->newShot(gunSpeed, posX - 10, posY - 10, 1);
					bullets->newShot(gunSpeed, posX + 13, posY - 15, 0);
					// if more than 100 points, double shot :D
				} else if (player->gun == 1) {
					bullets->newShot(gunSpeed, posX + 3, posY - 15, 0);
					bullets->newShot(gunSpeed, posX + 23, posY - 15, 0);
					// less than 100 points, stays with the single shot :|
				} else {
					bullets->newShot(gunSpeed, posX + 13, posY - 15, 0);
				}

				if (player->bulletNumber == 0) {
					// if powerup speed gun, decrease weapon gun
					if (player->gunSpeed < 430) {
						player->gunSpeed = player->gunSpeed + 100;
					} else {
						player->gun--;
					}
					player->bulletNumber = 100;
				}

			}
		}
	}

}
