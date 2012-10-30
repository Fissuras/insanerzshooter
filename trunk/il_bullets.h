#ifndef il_bullets_h
#define il_bullets_h

#include "global.h"
#include "bullet.h"
using namespace std;
using std::vector;

class IL_Bullets {

public:

	vector<Bullet> bullets;
	IL_Sprite spriteTiro;

	IL_Bullets() {
	}

	~IL_Bullets() {
	    bullets.clear();
	}

	void newShot(int gunSpeed, int x, int y, int type);

	void draw(SDL_Surface *tela);

	void updatePositions();

};

#endif
