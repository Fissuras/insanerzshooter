#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#ifdef ANDROID
	#include "SDL.h"
#else
	#include <SDL/SDL.h>
#endif

//constants
extern int NUMBER_OF_BACKGROUND_STARS;
extern int NUMBER_OF_PARTICLES_ENEMY_EXPLOSION;
extern int PLAYER_SCORE;
extern int HISCORE;
extern bool PLAYER_ALIVE;
extern bool GAME_PAUSED;
extern int PLAYER_LIVES;
extern SDL_Color whiteColor;
extern SDL_Color yellowColor;

//runtime variables
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int TITLE_X;
extern int TITLE_Y;
extern int SMALL_FONT_SIZE;
extern int NORMAL_FONT_SIZE;

extern double Abs(double number);// Get the absolute number

#endif
