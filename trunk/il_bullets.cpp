#include "il_bullets.h"

// Cria um novo bullet e o armazena no vetor de bullets
void IL_Bullets::novoTiro(int gunSpeed, int x, int y, int type) {
	Bullet bullet = Bullet(gunSpeed,x,y, spriteTiro, type);
	bullets.push_back(bullet);
}

// Desenha os bullets na tela/surface
void IL_Bullets::draw(SDL_Surface *tela) {
	for (unsigned int i = 0; i < bullets.size(); i++) {
		Bullet bullet = bullets.at(i);
		bullet.draw(tela);
	}
}

// Move todos os bullets para cima
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
