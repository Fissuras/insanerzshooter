#ifndef particlessystem_h
#define particlessystem_h

#include <vector>
#include "global.h"
#include "particle.h"
using namespace std;
using std::vector;

class ParticlesSystem {

public:

	vector<Particle> particleGroup;
	int lifeTime, maxLifeTime;

	ParticlesSystem() {
	    lifeTime = 0;
	    maxLifeTime = 0;
	}

	ParticlesSystem(int x, int y, int newType, int particleNumber, int newMaxLifeTime) {

	    lifeTime = 0;
	    maxLifeTime = newMaxLifeTime;

	    if (newType == 0) {
	        for (int i = 0; i < particleNumber; i++) {
	            Particle p = Particle(x, y, newType);
	            particleGroup.push_back(p);
	        }
	    } else if (newType == 1) {

	        Particle p = Particle(x + rand()%8 + 11,y + 28, newType);
	        particleGroup.push_back(p);

	    } else if (newType == 2) {

	        Particle p = Particle(x, y, newType);
	        particleGroup.push_back(p);

	    } else if (newType == 3) {

	        Particle p = Particle(x + rand()%8 + 6, y, newType);
	        particleGroup.push_back(p);

	    }

	}

	~ParticlesSystem() {
	    particleGroup.clear();
	}

	void draw(SDL_Surface *tela);

};

#endif
