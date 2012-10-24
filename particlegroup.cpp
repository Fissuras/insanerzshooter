#include "particlegroup.h"

void ParticleGroup::addParticleSystem(int x, int y, int type, int quantidadeDeParticulas, int tempoDeVidaMaximo) {
	ParticlesSystem s = ParticlesSystem(x, y, type, quantidadeDeParticulas, tempoDeVidaMaximo);
	group.push_back(s);
}

void ParticleGroup::draw(SDL_Surface *tela) {
	for (unsigned int i = 0; i < group.size(); i++) {
		group.at(i).draw(tela);
		if (group.at(i).tempoDeVidaMaximo != -1 && group.at(i).tempoDeVida > group.at(i).tempoDeVidaMaximo) {
			group.erase(group.begin() + i);
		}
	}
}
