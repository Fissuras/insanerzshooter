#include "il_player.h"

void IL_Player::restart() {
	PLAYER_SCORE = 0;
	gun = 0;
	gunSpeed = 430;
	bulletNumber = 100;
	speed = 1.5;
	playerSprite.position.x = (SCREEN_WIDTH / 2) - playerSprite.surface->w;
    playerSprite.position.y = SCREEN_HEIGHT - (2 * playerSprite.surface->h);
}
