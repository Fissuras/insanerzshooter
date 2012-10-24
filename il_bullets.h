#ifndef il_bullets_h
#define il_bullets_h
	#include "global.h"
	#include "bullet.h"
	using namespace std;
	using std::vector;

	// Um conjunto de bullets, onde todos tem a mesma imagem/surface
	class IL_Bullets {

	public:

		vector<Bullet> bullets;     // vetor onde os bullets serão armazenados
		IL_Sprite spriteTiro;   // Sprite representando os bullets

		// Construtor padrão
		IL_Bullets() {
		}

		~IL_Bullets() {
		    bullets.clear();
		}

		// Cria um novo bullet e o armazena no vetor de bullets
		void novoTiro(int gunSpeed, int x, int y, int type);

		// Desenha os bullets na tela/surface
		void draw(SDL_Surface *tela);

		// Move todos os bullets para cima
		void updatePositions();

	};
#endif
