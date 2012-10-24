#ifndef bullet_h
#define bullet_h
	#include "il_position.h"
	#include "il_sprite.h"

	// Representa um bullet do player.
	class Bullet {

	public:

		int type;
		IL_Position position;
		Uint32 corTiroVelLenta;
		Uint32 corTiroVelMedia;
		Uint32 corTiroVelRapida;
		int gunSpeed;
		int width, height;

		Bullet(int gunSpeed, int x, int y, IL_Sprite  spriteTiroNovo, int typeNovo) {
		    position.x = x;
		    position.y = y;
		    type = typeNovo;
		    width = 6;
		    height = 12;
		    gunSpeed = gunSpeed;
		}

		~Bullet() {}

		void draw(SDL_Surface *tela);

		void updatePosition();

	};
#endif
