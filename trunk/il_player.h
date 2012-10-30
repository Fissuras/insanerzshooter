#ifndef il_player_h
#define il_player_h

#include "il_sprite.h"
#include "timer.h"
#include "global.h"

class IL_Player {

public:

	IL_Sprite playerSprite;
	int bulletNumber;
	int gun;
	int gunSpeed;
	float speed;
	Timer *deathTimer;  //timer delay between death and game restart

	IL_Player() {
	    speed = 1.5;
	    gun = 0;
	    gunSpeed = 430;
	    deathTimer = new Timer();
	    deathTimer->start();
	    bulletNumber = 100;
	}

	IL_Player(IL_Sprite newPlayerSprite) {
	    deathTimer = new Timer();
	    deathTimer->start();
	    playerSprite = newPlayerSprite;
	    gun = 0;
	    gunSpeed = 430; // bullet delay (< is faster)
	    bulletNumber = 100;
	    speed = 1.5;
		playerSprite.position.x = (SCREEN_WIDTH / 2) - playerSprite.surface->w;
    	playerSprite.position.y = SCREEN_HEIGHT - (2 * playerSprite.surface->h);
	}

	~IL_Player() {
	    delete deathTimer;
	    deathTimer = NULL;
	}

	void restart();

};

#endif
