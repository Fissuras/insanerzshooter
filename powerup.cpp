#include "powerup.h"

void PowerUp::act() {

	if (!GAME_PAUSED) {
		position.y = position.y + 0.5;
	}

}
