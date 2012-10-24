#ifndef il_player_h
#define il_player_h
	#include "il_sprite.h"
	#include "timer.h"
	class IL_Player {

	public:

		IL_Sprite playerSprite;
		int bulletNumber;
		int gun;
		int gunSpeed;
		float speed;
		Timer *deathTimer;  // Timer para dar um "delay" entre a morte e a possibilidade de reiniciar o jogo

		IL_Player() {
		    speed = 1.5;
		    gun = 0;
		    gunSpeed = 430;
		    deathTimer = new Timer();
		    deathTimer->start();
		    bulletNumber = 100;
		}

		IL_Player(IL_Sprite playerSprite) {
		    deathTimer = new Timer();
		    deathTimer->start();
		    playerSprite = playerSprite;
		    gun = 0;
		    gunSpeed = 430; // é o tempo de delay entre os bullets - quanto menor o valor, mais rapida é
		    bulletNumber = 100;
		    speed = 1.5;
   			playerSprite.position.x = (SCREEN_WIDTH / 2) - playerSprite.surface->w;
		    playerSprite.position.y = SCREEN_HEIGHT - (2 * playerSprite.surface->h);
		}

		~IL_Player() {
		    delete deathTimer;
		    deathTimer = NULL;
		}

		void reiniciar();

	};
#endif
