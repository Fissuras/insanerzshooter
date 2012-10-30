#include "il_bullets.h"

void IL_Bullets::newShot(int gunSpeed, int x, int y, int type) {
	Bullet bullet = Bullet(gunSpeed,x,y, spriteTiro, type);
	bullets.push_back(bullet);
}

void IL_Bullets::draw(SDL_Surface *tela) {
	for (unsigned int i = 0; i < bullets.size(); i++) {
		Bullet bullet = bullets.at(i);
		bullet.draw(tela);
	}
}

void IL_Bullets::updatePositions() {
	if (!GAME_PAUSED) {
		for (unsigned int i = 0; i < bullets.size(); i++) {
			bullets.at(i).updatePosition();
			if (bullets.at(i).position.y  < 50) {
				bullets.erase(bullets.begin() + i);
			}
		}
	}
}
