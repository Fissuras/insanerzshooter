#ifndef enemy_h
#define enemy_h

#include "global.h"
#include "il_sprite.h"
#include <math.h>

class Enemy : public IL_Sprite {

public:

	float speed;
	float seno; // used to move the enemy
	int type;

	Enemy() {
	    position.y = -250; // always appears up screen
	    speed = rand()%100;
	    speed = (speed / 100) + 1; // random speed (between 1 and 2)
	    seno = (rand()%100 / 100) - 0.5; // random sin (between -0.5 and +0.5)
	    type = 0; // each type has own movement (act method)
	}

	~Enemy() {}

	Enemy(IL_Sprite novoSprite, int typeNovo) {
	    type = typeNovo;
	    surface = novoSprite.surface;
	    position.x = rand() % int((SCREEN_WIDTH - (SCREEN_WIDTH * 0.087)));
	    position.y = -250;
	    speed = rand()%100;
	    speed = (speed / 100) + 1;
	    seno = (rand()%100 / 100) - 0.5;
	}

	void act();

};

#endif
