#include "enemy.h"

void Enemy::act() {

	if (GAME_PAUSED) {
		return;
	}

	if (type == 0) {// 0 = straight
		position.y = position.y + speed;

	} else if (type == 1) {// 1 = goes right
		position.y = position.y + speed;
		position.x = position.x + (sin(seno)) + 1;
		seno = seno + 0.1;
		if (position.x + 29 > SCREEN_WIDTH) {
			type = 2;
		}

	} else if (type == 2) {// 2 = goes left
		position.y = position.y + speed;
		position.x = position.x + (sin(seno)) - 1;
		seno = seno + 0.1;
		if (position.x < 0) {
			type = 1;
		}

	} else if (type == 3) {// 3 = short clockwise spin than go straight
		if (position.y < 200) {
			position.x = position.x + (cos(seno) * 5);
			position.y = position.y + (sin(seno) * 5) + speed;
			seno = seno + 0.1;
		} else {
			position.y = position.y + speed;
		}

	} else if (type == 4) {// 4 = short anti clockwise spin
		position.x = position.x - (cos(seno) * 5);
		position.y = position.y + (sin(seno) * 5) + speed;
		seno = seno + 0.1;

	} else if (type == 5) {// 5 = short clockwise spin
		position.x = position.x + (cos(seno) * 5);
		position.y = position.y + (sin(seno) * 5) + speed;
		seno = seno + 0.1;

	} else if (type == 6) {// 6 = long clockwise spin
		position.x = position.x + (cos(seno) * 5);
		position.y = position.y + (sin(seno) * 5) + speed;
		seno = seno + 0.03;

	} else if (type == 7) {// 7 = long anti clockwise spin
		position.x = position.x - (cos(seno) * 5);
		position.y = position.y + (sin(seno) * 5) + speed;
		seno = seno + 0.03;
	}

	// Out of screen = reposition the enemy
	if ( position.y > int(SCREEN_HEIGHT - surface->h) ) {
		position.y = -250;
		position.x = rand() % int(SCREEN_WIDTH - (SCREEN_WIDTH * 0.125));
		seno = 0;
	}

}
