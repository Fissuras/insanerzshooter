#include "global.h"

using namespace std;
using std::vector;

int NUMBER_OF_BACKGROUND_STARS = 50;
int NUMBER_OF_PARTICLES_ENEMY_EXPLOSION = 30;
int PLAYER_SCORE = 0;
int HISCORE = 0;
bool PLAYER_ALIVE = false;
bool GAME_PAUSED = false;
int PLAYER_LIVES = 0;

SDL_Color whiteColor = {255, 255, 255};
SDL_Color yellowColor = {255, 255, 0};

#ifdef PSP
	int SCREEN_WIDTH = 480;
	int SCREEN_HEIGHT = 272;
	int TITLE_X = 135;
	int TITLE_Y = 50;
	int SMALL_FONT_SIZE = 16;
	int NORMAL_FONT_SIZE = 18;
#elif ANDROID
	int SCREEN_WIDTH = 320;
	int SCREEN_HEIGHT = 440;
	int TITLE_X = SCREEN_WIDTH/2 - 20;
	int TITLE_Y = SCREEN_HEIGHT/2 - 20;
	int SMALL_FONT_SIZE = 10;
	int NORMAL_FONT_SIZE = 12;
#else
//	int SCREEN_WIDTH = 640;
//	int SCREEN_HEIGHT = 480;
	int SCREEN_WIDTH = 320;
	int SCREEN_HEIGHT = 240;
	int TITLE_X = 310;
	int TITLE_Y = 200;
//	int SMALL_FONT_SIZE = 12;
//	int NORMAL_FONT_SIZE = 14;
	int SMALL_FONT_SIZE = 8;
	int NORMAL_FONT_SIZE = 10;
#endif

double Abs(double number) {
    if ( number >= 0 ) {
        return number;
    } else {
        return -number;
    }
}
