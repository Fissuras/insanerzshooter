#ifndef enemygroup_h
#define enemygroup_h

#include "global.h"
#include "enemy.h"
#include "particlegroup.h"
#include "il_bullets.h"
#include "il_player.h"
#ifdef ANDROID
	#include "SDL_mixer.h"
#else
	#include <SDL/SDL_mixer.h>
#endif

using namespace std;
using std::vector;

class EnemyGroup {

public:

	vector<Enemy> enemies;

	EnemyGroup() { }

	~EnemyGroup() {
	    enemies.clear();
	}

	void add(Enemy sprite);

	void draw(SDL_Surface *tela);

	void act(SDL_Surface *tela);

	void actAndDraw(SDL_Surface *tela, ParticleGroup *group);

	// Create a new enemy with random type and position X
	void createNewEnemy(IL_Sprite spriteEnemy, int type);

	// Check the collision with bullet
	void checkCollision(IL_Bullets *bullets, ParticleGroup *group, Mix_Chunk * explosion);

	// Check the collision with player
	bool checkCollision(IL_Player *player);

};

#endif
