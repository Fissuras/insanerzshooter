#ifndef particlessystem_h
#define particlessystem_h
	#include <vector>
	#include "global.h"
	#include "particula.h"
	using namespace std;
	using std::vector;

	// ParticlesSystem
	class ParticlesSystem {

	public:

		vector<Particula> particulas;
		int tempoDeVida, tempoDeVidaMaximo;

		ParticlesSystem() {
		    tempoDeVida = 0;
		    tempoDeVidaMaximo = 0;
		}

		ParticlesSystem(int x, int y, int typeNovo, int numParticulas, int tempoDeVidaMaximoNovo) {

		    tempoDeVida = 0;
		    tempoDeVidaMaximo = tempoDeVidaMaximoNovo;

		    if (typeNovo == 0) {
		        for (int i = 0; i < numParticulas; i++) {
		            Particula p = Particula(x, y, typeNovo);
		            particulas.push_back(p);
		        }
		    } else if (typeNovo == 1) {

		        Particula p = Particula(x + rand()%8 + 11,y + 28, typeNovo);
		        particulas.push_back(p);

		    } else if (typeNovo == 2) {

		        Particula p = Particula(x, y, typeNovo);
		        particulas.push_back(p);

		    } else if (typeNovo == 3) {

		        Particula p = Particula(x + rand()%8 + 6, y, typeNovo);
		        particulas.push_back(p);

		    }

		}

		~ParticlesSystem() {
		    particulas.clear();
		}

		void draw(SDL_Surface *tela);

	};
#endif
