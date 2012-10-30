#ifndef particlegroup_h
#define particlegroup_h

#include "particlessystem.h"

class ParticleGroup {

public:

	vector<ParticlesSystem> group;

	ParticleGroup() {}

	~ParticleGroup() {
	    group.clear();
	}

	void addParticleSystem(int x, int y, int type, int numberOfParticles, int maxLifeTime);

	void draw(SDL_Surface *tela);

};

#endif
