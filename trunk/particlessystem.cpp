#include "particlessystem.h"

void ParticlesSystem::draw(SDL_Surface *tela) {
	for (unsigned int i = 0; i < particleGroup.size(); i++) {
		if (!GAME_PAUSED) {
			particleGroup.at(i).updatePosition();
		}
		particleGroup.at(i).draw(tela);
	}
	if (!GAME_PAUSED) {
		lifeTime++;
	}
}
