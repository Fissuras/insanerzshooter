#include "particlessystem.h"

void ParticlesSystem::draw(SDL_Surface *tela) {
	for (unsigned int i = 0; i < particulas.size(); i++) {
		if (!GAME_PAUSED) {
			particulas.at(i).updatePosition();
		}
		particulas.at(i).draw(tela);
	}
	if (!GAME_PAUSED) {
		tempoDeVida++;
	}
}
