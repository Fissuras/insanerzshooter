#ifndef il_inputhandle_h
#define il_inputhandle_h

#include "global.h"
#include "il_screen.h"
#include "il_player.h"
#include "il_bullets.h"
#include "enemygroup.h"
#include "powerupsgroup.h"
#ifdef ANDROID
	#include "SDL.h"
	#include "SDL_mixer.h"
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_mixer.h>
#endif

// Handle all input
class IL_InputHandle {

public:

	bool leftPressed, rightPressed, upPressed, downPressed, shotButtonPressed, pausePressed;
	EnemyGroup *enemiesGroup;//reference to restart the game
	PowerUpsGroup *powerupsGroup;//reference to restart the game
	IL_Bullets *bullets;//reference to create a new bullet
	Timer *shotTimer;//delay gun shot
	IL_Screen *screen;
	SDL_Joystick *joystick;
	Mix_Chunk *laser;

	IL_InputHandle(IL_Screen *newScreen, IL_Player *newPlayer, IL_Bullets *newBullets, EnemyGroup *newEnemiesGroup, PowerUpsGroup *newPowerUpsGroup, Mix_Chunk *newLaser) {
	    player = newPlayer;
	    bullets = newBullets;
	    enemiesGroup = newEnemiesGroup;
	    powerupsGroup = newPowerUpsGroup;
	    leftPressed = false;
	    rightPressed = false;
	    upPressed = false;
	    downPressed = false;
	    shotButtonPressed = false;
	    pausePressed = false;
	    shotTimer = new Timer();
	    shotTimer->start();
	    screen = newScreen;
	    joystick = SDL_JoystickOpen(0);
	    laser = newLaser;
	}

	~IL_InputHandle() {
	    SDL_JoystickClose(joystick);
	}

	//Only check if any button pressed
	void checkPressed();

	//Execute the actions
	void handleActions();

private:

	IL_Player *player;
	SDL_Event event;

};

#endif
