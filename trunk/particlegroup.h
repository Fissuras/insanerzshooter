#ifndef particlegroup_h
#define particlegroup_h
	#include "particlessystem.h"

	// ParticleGroup
	class ParticleGroup {

	public:

		vector<ParticlesSystem> group;

		ParticleGroup() {}

		~ParticleGroup() {
		    group.clear();
		}

		void addParticleSystem(int x, int y, int type, int quantidadeDeParticulas, int tempoDeVidaMaximo);
	
		void draw(SDL_Surface *tela);

	};
#endif
