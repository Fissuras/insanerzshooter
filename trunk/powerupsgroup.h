#ifndef powerupsgroup_h
#define powerupsgroup_h

#include "global.h"
#include "powerup.h"
#include "il_player.h"
#ifdef ANDROID
	#include "SDL_mixer.h"
#else
	#include <SDL/SDL_mixer.h>
#endif
using namespace std;
using std::vector;

class PowerUpsGroup {

public:

	vector<PowerUp> powerupsGroup;
	IL_Player * player;

	PowerUpsGroup(IL_Player * playerNovo) {
	    player = playerNovo;
	}

	~PowerUpsGroup() {
	    powerupsGroup.clear();
	}

	void add(PowerUp powerUp);

	void actAndDraw(SDL_Surface *tela);

	void checkCollision(Mix_Chunk * powerup, Mix_Chunk * doubleshoot, Mix_Chunk * tripleshoot, Mix_Chunk * insaneshoot, Mix_Chunk * speedup);

};

#endif
