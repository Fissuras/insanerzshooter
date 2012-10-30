#include "particlegroup.h"

void ParticleGroup::addParticleSystem(int x, int y, int type, int numberOfParticles, int maxLifeTime) {
	ParticlesSystem s = ParticlesSystem(x, y, type, numberOfParticles, maxLifeTime);
	group.push_back(s);
}

void ParticleGroup::draw(SDL_Surface *tela) {
	for (unsigned int i = 0; i < group.size(); i++) {
		group.at(i).draw(tela);
		if (group.at(i).maxLifeTime != -1 && group.at(i).lifeTime > group.at(i).maxLifeTime) {
			group.erase(group.begin() + i);
		}
	}
}
