#ifndef powerup_h
#define powerup_h

#include "global.h"
#include "il_sprite.h"

class PowerUp : public IL_Sprite {

public:

	float speed;
	int type;

	PowerUp() {
	    position.y = -250;
	    speed = rand()%100;
	    speed = (speed / 100) + 1;
	    type = 0;
	}

	~PowerUp() {}

	PowerUp(IL_Sprite novoSprite, int typeNovo) {
	    type = typeNovo;
	    surface = novoSprite.surface;
	    position.x = rand() % int(SCREEN_WIDTH - (SCREEN_WIDTH * 0.125));
	    position.y = -250;
	    speed = rand()%100;
	    speed = (speed / 100) + 1;
	}

	void act();

};

#endif
