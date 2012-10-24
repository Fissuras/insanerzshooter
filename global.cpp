#include "global.h"

using namespace std;
using std::vector;

int NUMBER_OF_BACKGROUND_STARS = 50;
int NUMBER_OF_PARTICLES_ENEMY_EXPLOSION = 30;
int PLAYER_SCORE = 0;
int HISCORE = 0;
bool PLAYER_ALIVE = false;
bool GAME_PAUSED = false;

#ifdef PSPFW3X
	int SCREEN_WIDTH = 480;
	int SCREEN_HEIGHT = 272;
	int TITLE_X = 135;
	int TITLE_Y = 50;
	int FRAMES_PER_SECOND = 360;
#else
	int SCREEN_WIDTH = 800;
	int SCREEN_HEIGHT = 600;
	int TITLE_X = 310;
	int TITLE_Y = 200;
	int FRAMES_PER_SECOND = 160;
#endif
