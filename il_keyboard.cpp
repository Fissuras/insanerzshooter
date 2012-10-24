#include "il_keyboard.h"
#include <SDL/SDL.h>

// Verifica se algum tecla foi pressionada ou solta.
// Caso usuario tenha pressionado uma tecla, sua variavel respectiva torna-se true. Ficará como false caso o usuário solte a tecla.
void IL_Keyboard::verificaTeclasPressionadas() {

	if ( SDL_PollEvent(&event) != 0 ) {

		//Verificamos o type do evento
		switch (event.type) {
			// Caso tenha fechado a janela
		case SDL_QUIT:
			exit(0); //Fechamos a aplicação
			break;
		default:
			break;

			// Caso tenha pressionado uma tecla qualquer
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
			case SDLK_F11:
				F11pressed = true;
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
				spacePressed = true;
				break;
			default:
				break;
			}
			break;

			// Caso tenha soltado uma tecla qualquer
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
				spacePressed = false;
				break;
			case SDLK_F11:
				F11pressed = false;
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

			#ifdef psp
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
				spacePressed = true;
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
				spacePressed = false;
				break;
			default:
				break;
			}
			break;
			#endif
		}
	}
}

// O método verificaTeclasPressionadas() apenas seta as variaveis para true ou false.
// O método acoesDoTeclado é o responsável por executar de fato as ações
void IL_Keyboard::acoesDoTeclado() {
	if (F11pressed == true) {
		screen->estaEmFullScreen = !screen->estaEmFullScreen;
		screen = new IL_Screen(screen->estaEmFullScreen); // Cria uma nova tela
		F11pressed = false;
	} else if (pausePressed == true) {
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
	if (!GAME_PAUSED && spacePressed == true) {

		if (PLAYER_ALIVE == false) {
			if (player->deathTimer->get_ticks() > 5000) {
				// reiniciando jogo!
				PLAYER_ALIVE = true;
				player->reiniciar();
				groupDeEnemys->enemies.clear();
				groupDePowerUps->powerups.clear();
				spacePressed  = false; // evita bug de atirar quando inicia jogo
			}
		} else {
			// verifica se ja deu tempo para atirar denovo
			if (shootTimer->get_ticks() > player->gunSpeed) {
				Mix_PlayChannel(-1, laser, 0);
				shootTimer->start();
				// Diminui o numero de balas (desde que não seja a gun inicial)
				if (player->gun != 0 || player->gunSpeed < 430) {
					player->bulletNumber--;
				}

				int gunSpeed = 0;
				if (player->gunSpeed == 330) {
					gunSpeed = 1;
				} else if (player->gunSpeed == 230) {
					gunSpeed = 2;
				}

				// Se já tiver mais de 300 playerScore, liberamos o bullet sextuplo! :D
				int posX = player->playerSprite.position.x;
				int posY = player->playerSprite.position.y;
				if (player->gun == 3) {
					bullets->novoTiro(gunSpeed, posX + 42, posY - 10, 2);
					bullets->novoTiro(gunSpeed, posX + 32, posY - 10, 2);
					bullets->novoTiro(gunSpeed, posX - 20, posY - 10, 1);
					bullets->novoTiro(gunSpeed, posX - 10, posY - 10, 1);
					bullets->novoTiro(gunSpeed, posX + 3, posY - 15, 0);
					bullets->novoTiro(gunSpeed, posX + 23, posY - 15, 0);
					// Se já tiver mais de 200 playerScore, liberamos o bullet triplo! :D
				} else if (player->gun == 2) {
					bullets->novoTiro(gunSpeed, posX + 32, posY - 10, 2);
					bullets->novoTiro(gunSpeed, posX - 10, posY - 10, 1);
					bullets->novoTiro(gunSpeed, posX + 13, posY - 15, 0);
					// Se já tiver mais de 100 playerScore, liberamos o bullet duplo! :D
				} else if (player->gun == 1) {
					bullets->novoTiro(gunSpeed, posX + 3, posY - 15, 0);
					bullets->novoTiro(gunSpeed, posX + 23, posY - 15, 0);
					// Se tiver menos que 100 playerScore, terá apenas o bullet normal
				} else {
					bullets->novoTiro(gunSpeed, posX + 13, posY - 15, 0);
				}

				// Verfica se acabaram as balas!
				if (player->bulletNumber == 0) {
					// se estiver com powerup de speed, aumenta intervalo entre os bullets
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
