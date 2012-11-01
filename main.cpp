/*  Copyright 2008 Insanerz
    This file is part of Insanerz Shooter.

    Insanerz Shooter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Insanerz Shooter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Insanerz Shooter.  If not, see <http://www.gnu.org/licenses/>. */

// To compile it for GNU/Debian Linux and Ubuntu, please, replace "data/" with "/usr/share/insanerzshooter/"
// After that, run the createDebianPkg.sh script. The .deb package will be created. Have fun!

#include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>
#include <string>
#include <math.h>
#include <fstream>

#ifdef WII
	#include <gccore.h>
	#include <WIIuse/wpad.h>
	#include <fat.h>
#endif

#ifdef ANDROID
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_ttf.h"
	#include "SDL_mixer.h"
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_ttf.h>
	#include <SDL/SDL_mixer.h>
#endif

#ifdef ANDROID
    #define DIR_CUR "/sdcard/"
#else
    #define DIR_CUR ""
#endif

#define DIR_SEP "/"
#define DATAFILE(X)     DIR_CUR "data" DIR_SEP X
#define	FRAMES_PER_SEC	60

#include "global.h"

#include "timer.h"
#include "il_position.h"
#include "il_sprite.h"

#include "bullet.h"
#include "il_bullets.h"

#include "particle.h"
#include "particlegroup.h"
#include "particlessystem.h"

#include "enemy.h"
#include "enemygroup.h"

#include "il_animatedtext.h"
#include "il_inputhandle.h"
#include "il_player.h"
#include "il_screen.h"

#include "powerup.h"
#include "powerupsgroup.h"

#ifdef PSP
    #include <pspkernel.h>
    #include <pspdebug.h>
    #include <pspsdk.h>
    #include <psppower.h>

    PSP_MODULE_INFO("InsanerzShooter", 0, 1, 1);
    PSP_HEAP_SIZE_KB(12*1024);

    int exit_callback(int arg1, int arg2, void *common) {
        sceKernelExitGame();
        return 0;
    }

    int CallbackThread(SceSize args, void *argp) {
        int cbid;
        cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
        sceKernelRegisterExitCallback(cbid);
        sceKernelSleepThreadCB();
        return 0;
    }

    int SetupCallbacks(void) {
        int thid = 0;
        thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
        if (thid >= 0) {
            sceKernelStartThread(thid, 0, 0);
        }
        return thid;
    }
#endif

void waitFrame(void) {
	static Uint32 next_tick = 0;
	Uint32 this_tick;

	this_tick = SDL_GetTicks();
	if ( this_tick < next_tick ) {
		SDL_Delay(next_tick-this_tick);
	}
	next_tick = this_tick + (1000/FRAMES_PER_SEC);
}

int main(int argc, char *args[]) {

    #ifdef PSP
	    scePowerSetClockFrequency(333, 333, 166);
    	SetupCallbacks();
        atexit(sceKernelExitGame);
    #else
	    atexit(SDL_Quit);
    #endif

	// ==================================INITIALIZE EVERYTHING==================================
    srand(time(NULL));
    char playerScoreRuntime[5];
    char *playerHiscoreRuntime;
    playerHiscoreRuntime = new char[5];
	int probDeCriarEnemy;// calc to create a enemy
    IL_Screen *screen = new IL_Screen();
    SDL_ShowCursor(false);
    SDL_WM_SetCaption("Insanerz Shooter", NULL);
    TTF_Init();

	// =======================================LOAD HISCORE======================================
    FILE *pFile = fopen(DATAFILE("hiscore.dat"), "rt");
    if (pFile != NULL) {
        fgets(playerHiscoreRuntime, 6, pFile);
        HISCORE = strtol(playerHiscoreRuntime, NULL, 10);
        fclose(pFile);
    } else {
        HISCORE = 0;
    }

    // ======================================MUSIC & SNDFX======================================
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1200);
    Mix_AllocateChannels(MIX_DEFAULT_CHANNELS);
    Mix_Music *musica = Mix_LoadMUS(DATAFILE("musica.wav"));
    Mix_Chunk *powerup = Mix_LoadWAV(DATAFILE("powerup.wav"));
    Mix_Chunk *doubleshoot = Mix_LoadWAV(DATAFILE("doubleshot.wav"));
    Mix_Chunk *tripleshoot = Mix_LoadWAV(DATAFILE("tripleshot.wav"));
    Mix_Chunk *insaneshoot = Mix_LoadWAV(DATAFILE("insaneshot.wav"));
    Mix_Chunk *laser = Mix_LoadWAV(DATAFILE("laser.wav"));
    Mix_Chunk *explosion = Mix_LoadWAV(DATAFILE("explosion.wav"));
    Mix_Chunk *playerExplosion = Mix_LoadWAV(DATAFILE("bomb.wav"));
    Mix_Chunk *speedup = Mix_LoadWAV(DATAFILE("speedup.wav"));
    Mix_PlayMusic(musica, -1);

	// ======================================PLAYER & ENEMY=====================================
    IL_Sprite playerSprite(DATAFILE("player.png"), 2);
    IL_Player *player = new IL_Player(playerSprite);
    IL_Bullets *bullets = new IL_Bullets();
    IL_Sprite spriteEnemy(DATAFILE("enemy1.png"), 2);
    IL_Sprite spriteEnemy2(DATAFILE("enemy2.png"), 2);
    IL_Sprite spriteEnemy3(DATAFILE("enemy3.png"), 2);
    EnemyGroup enemiesGroup = EnemyGroup();

	// =========================================POWERUPS========================================
    PowerUpsGroup powerupsGroup = PowerUpsGroup(player);
    IL_Sprite powerUp1Sprite(DATAFILE("powerup1.png"), 2);
    IL_Sprite powerUp2Sprite(DATAFILE("powerup2.png"), 2);

    IL_InputHandle inputHandle = IL_InputHandle(screen, player, bullets, &enemiesGroup, &powerupsGroup, laser);

	#ifdef ANDROID
    	TTF_Font *normalFont = TTF_OpenFont(DATAFILE("FreeSans_bold.ttf"), 12);
    	TTF_Font *smallFont = TTF_OpenFont(DATAFILE("FreeSans_bold.ttf"), 10);
	#else
    	TTF_Font *normalFont = TTF_OpenFont(DATAFILE("FreeSans_bold.ttf"), 20);
		TTF_Font *smallFont = TTF_OpenFont(DATAFILE("FreeSans_bold.ttf"), 16);
	#endif

    // ==========================================SCORE==========================================
    SDL_Surface *scoreSurface = TTF_RenderText_Solid(normalFont, "SCORE", yellowColor);
    SDL_Rect scoreRect;
    scoreRect.x = 20;
    scoreRect.y = 0;
    SDL_Surface *playerScoreSurface = TTF_RenderText_Solid(normalFont, "0", whiteColor);
    SDL_Rect *playerScoreRect = new SDL_Rect();
    playerScoreRect->x = 20;
    playerScoreRect->y = 20;

    // =========================================HISCORE=========================================
    SDL_Surface *hiscoreSurface = TTF_RenderText_Solid(normalFont, "HISCORE", yellowColor);
    SDL_Rect hiscoreRect;
    hiscoreRect.x = SCREEN_WIDTH - hiscoreSurface->w - 20;
    hiscoreRect.y = 0;
    SDL_Surface *hiscoreNUMSurface = TTF_RenderText_Solid(normalFont, "0", whiteColor);
    SDL_Rect *hiscoreNUMRect = new SDL_Rect();
    hiscoreNUMRect->x = SCREEN_WIDTH - hiscoreSurface->w - 20;
    hiscoreNUMRect->y = 20;

    // ==========================================PAUSE==========================================
    SDL_Surface *pauseSurface = TTF_RenderText_Solid(normalFont, "P A U S E", whiteColor);
    SDL_Rect pauseRect;
    pauseRect.x = (SCREEN_WIDTH - pauseSurface->w) / 2;
    pauseRect.y = (SCREEN_HEIGHT - pauseSurface->h) / 2;

    // =========================================BULLETS=========================================
    SDL_Surface *bulletSurface = TTF_RenderText_Solid(normalFont, "BULLETS", yellowColor);
    SDL_Rect bulletRect;
    bulletRect.x = SCREEN_WIDTH * 0.4;
    bulletRect.y = 20;

    // ===========================================URL===========================================
    SDL_Surface *urlSurface = TTF_RenderText_Solid(smallFont, "http://insanerzshooter.googlepages.com", yellowColor);
    SDL_Rect urlRect;
    urlRect.x = 0;
    urlRect.y = 0;

	// ==========================================START==========================================
    SDL_Surface *pressFireSurface = TTF_RenderText_Solid(smallFont, "Press FIRE to start", yellowColor);
    SDL_Rect pressFireRect;
    pressFireRect.x = (SCREEN_WIDTH - pressFireSurface->w) / 2;
    pressFireRect.y = int(SCREEN_HEIGHT - (2 * 16));

    // ==========================================LOGO===========================================
    IL_AnimatedText insanerz[8];
    insanerz[0] = IL_AnimatedText("I", 0,   0, false, normalFont);
    insanerz[1] = IL_AnimatedText("N", 30,  20, false, normalFont);
    insanerz[2] = IL_AnimatedText("S", 60,  40, false, normalFont);
    insanerz[3] = IL_AnimatedText("A", 90,  60, false, normalFont);
    insanerz[4] = IL_AnimatedText("N", 120, 40, false, normalFont);
    insanerz[5] = IL_AnimatedText("E", 150, 20, false, normalFont);
    insanerz[6] = IL_AnimatedText("R", 180, 0, false, normalFont);
    insanerz[7] = IL_AnimatedText("Z", 210, 20, false, normalFont);
    SDL_Rect insanerzRect;
    IL_AnimatedText shooter[7];
    shooter[0] = IL_AnimatedText("S", 0,   0, false, normalFont);
    shooter[1] = IL_AnimatedText("H", 30,  20, false, normalFont);
    shooter[2] = IL_AnimatedText("O", 60,  40, false, normalFont);
    shooter[3] = IL_AnimatedText("O", 90,  60, false, normalFont);
    shooter[4] = IL_AnimatedText("T", 120, 40, false, normalFont);
    shooter[5] = IL_AnimatedText("E", 150, 20, false, normalFont);
    shooter[6] = IL_AnimatedText("R", 180, 0, false, normalFont);

	// =======================================BACKGROUND========================================
    ParticleGroup *backgroundGroup = new ParticleGroup();
    for(int i = 0; i < NUMBER_OF_BACKGROUND_STARS; i++) {
        backgroundGroup->addParticleSystem(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 2, 0, -1);
    }

    while(true) {

    	waitFrame();

        screen->clean();
        backgroundGroup->draw(screen->surface);
        screen->drawFirePowerBar(player);
        powerupsGroup.actAndDraw(screen->surface);
        enemiesGroup.checkCollision(bullets, backgroundGroup, explosion);
        enemiesGroup.actAndDraw(screen->surface, backgroundGroup);

        if (!GAME_PAUSED) {
            bullets->updatePositions();
        }
        bullets->draw(screen->surface);

		// =====================================GAME INPUT======================================
        inputHandle.checkPressed();
        inputHandle.handleActions();

        if (PLAYER_ALIVE) {
            powerupsGroup.checkCollision(powerup, doubleshoot, tripleshoot, insaneshoot, speedup);
            screen->draw(player->playerSprite);
            int probDeCriarParticulaDeFogo = 6 - ((int) player->speed);
            if (!GAME_PAUSED) {
                if (rand()%probDeCriarParticulaDeFogo == 1) {
                    backgroundGroup->addParticleSystem(player->playerSprite.position.x, player->playerSprite.position.y, 1, 0, 40);
                }
            } else {
                SDL_BlitSurface(pauseSurface, NULL, screen->surface, &pauseRect);
            }
			if (PLAYER_SCORE % 30 == 0) {
				PLAYER_LIVES++;
			}
			//========================================HEADER========================================
	        SDL_BlitSurface(scoreSurface, NULL, screen->surface, &scoreRect);
	        SDL_BlitSurface(bulletSurface, NULL, screen->surface, &bulletRect);
			// ========================================SCORE========================================
	        sprintf(playerScoreRuntime,"%i",PLAYER_SCORE);
	        #ifdef PSP
	        	playerScoreSurface = TTF_RenderText_Solid(smallFont, playerScoreRuntime, whiteColor);
	        #else
	        	playerScoreSurface = TTF_RenderText_Solid(normalFont, playerScoreRuntime, whiteColor);
	        #endif
			playerScoreRect->x = SCREEN_WIDTH * 0.05 + ((scoreSurface->w - playerScoreSurface->w) / 2);
	        SDL_BlitSurface(playerScoreSurface, NULL, screen->surface, playerScoreRect);
	        SDL_FreeSurface(playerScoreSurface);//********************************************************************************************************************
			// =======================================HISCORE=======================================
		    sprintf(playerHiscoreRuntime,"%i",HISCORE);
		    #ifdef PSP
			    hiscoreNUMSurface = TTF_RenderText_Solid(smallFont, playerHiscoreRuntime, whiteColor);
		    #else
			    hiscoreNUMSurface = TTF_RenderText_Solid(normalFont, playerHiscoreRuntime, whiteColor);
		    #endif
			hiscoreNUMRect->x = (SCREEN_WIDTH * 0.75) + ((hiscoreSurface->w - hiscoreNUMRect->w) / 2);
		    SDL_BlitSurface(hiscoreNUMSurface, NULL, screen->surface, hiscoreNUMRect);
		    SDL_FreeSurface(hiscoreNUMSurface);
			// ===================================CHECK COLLISION===================================
		    if (enemiesGroup.checkCollision(player)) {
				if (PLAYER_LIVES > 0) {
					PLAYER_LIVES--;
				} else {
				    Mix_PlayChannel(-1, playerExplosion, 0);
				    player->deathTimer->start();
				    PLAYER_ALIVE = false;
				    if (PLAYER_SCORE > HISCORE) {
				        HISCORE = PLAYER_SCORE;
						// ===============================ADD HISCORE===============================
				        FILE *pFile = fopen(DATAFILE("hiscore.dat"), "w+");
				        sprintf(playerHiscoreRuntime,"%i",HISCORE);
				        if (pFile != NULL) {
				            fputs(playerHiscoreRuntime, pFile);
				            fclose(pFile);
				        }
				    }
				    backgroundGroup->addParticleSystem(player->playerSprite.position.x, player->playerSprite.position.y, 0, 500, 400);
				}
		    }
        } else {
            for (int i = 0; i < 8; i++) {
                insanerzRect.x = insanerz[i].posicaoChar.x + TITLE_X;
                insanerzRect.y = insanerz[i].posicaoChar.y + TITLE_Y;
                SDL_BlitSurface(insanerz[i].surface, NULL, screen->surface, &insanerzRect);
                insanerz[i].animate();

                if (i < 7) {
                    insanerzRect.x = shooter[i].posicaoChar.x + TITLE_X + 10;
                    insanerzRect.y = shooter[i].posicaoChar.y + TITLE_Y + 100;
                    SDL_BlitSurface(shooter[i].surface, NULL, screen->surface, &insanerzRect);
                    shooter[i].animate();
                }
            }
            if (player->deathTimer->get_ticks() > 5000) {
                SDL_BlitSurface(pressFireSurface, NULL, screen->surface, &pressFireRect);
            }
            SDL_BlitSurface(urlSurface, NULL, screen->surface, &urlRect);
        }

        // ====================================CREATE ENEMY=====================================
        if (enemiesGroup.enemies.size() < 50 && !GAME_PAUSED) {
            if (PLAYER_SCORE < 100 && enemiesGroup.enemies.size() < 30) {
                probDeCriarEnemy = rand() % 100;
            } else if (PLAYER_SCORE < 300 && enemiesGroup.enemies.size() < 30) {
                probDeCriarEnemy = rand() % 90;
            } else if (PLAYER_SCORE < 500 && enemiesGroup.enemies.size() < 30) {
                probDeCriarEnemy = rand() % 70;
            } else if (PLAYER_SCORE < 1000 && enemiesGroup.enemies.size() < 30) {
                probDeCriarEnemy = rand() % 50;
            } else if (PLAYER_SCORE < 2000 && enemiesGroup.enemies.size() < 35) {
                probDeCriarEnemy = rand() % 30;
            } else if (PLAYER_SCORE < 3000 && enemiesGroup.enemies.size() < 40) {
                probDeCriarEnemy = rand() % 30;
            } else if (PLAYER_SCORE < 4000 && enemiesGroup.enemies.size() < 45) {
                probDeCriarEnemy = rand() % 25;
            } else if (enemiesGroup.enemies.size() < 50) {
                probDeCriarEnemy = rand() % 10;
            }

            if (probDeCriarEnemy == 1) {
                if (PLAYER_SCORE > 30) {
                    int typeRand = rand() % 4;
                    if (typeRand == 0) {
                        enemiesGroup.createNewEnemy(spriteEnemy3, typeRand);
                    } else {
                        enemiesGroup.createNewEnemy(spriteEnemy, typeRand);
                    }
                } else {
                    int typeRand = rand() % 3;
                    if (typeRand == 0) {
                        enemiesGroup.createNewEnemy(spriteEnemy3, typeRand);
                    } else {
                        enemiesGroup.createNewEnemy(spriteEnemy, typeRand);
                    }
                }

            } else if (probDeCriarEnemy == 2) {
                if (PLAYER_SCORE > 30 && PLAYER_SCORE < 150) {
                    if (rand() % 8 == 1) {
                        enemiesGroup.createNewEnemy(spriteEnemy2, 4 + rand() % 2);
                    }
                } else if (PLAYER_SCORE > 150) {
                    enemiesGroup.createNewEnemy(spriteEnemy2, 4 + rand() % 4);
                }
            }
        }

        // ====================================CREATE POWERUP===================================
        int probDeCriarPowerUp = rand() % 3000;
		if (!GAME_PAUSED) {
			if (probDeCriarPowerUp == 1) {
				PowerUp p = PowerUp(powerUp1Sprite, 0);
				powerupsGroup.add(p);
			} else if (probDeCriarPowerUp == 2) {
				if (player->speed < 4) {
					PowerUp p = PowerUp(powerUp2Sprite, 1);
					powerupsGroup.add(p);
				} else {
					PowerUp p = PowerUp(powerUp1Sprite, 0);
					powerupsGroup.add(p);
				}
			}
		}

	    SDL_BlitSurface(hiscoreSurface, NULL, screen->surface, &hiscoreRect);
        SDL_Flip(screen->surface);
    }

	// =======================================RELEASE ALL=======================================
    Mix_FreeChunk(powerup);
    Mix_FreeChunk(doubleshoot);
    Mix_FreeChunk(tripleshoot);
    Mix_FreeChunk(insaneshoot);
    Mix_FreeChunk(laser);
    Mix_FreeChunk(explosion);
    Mix_FreeChunk(playerExplosion);
    Mix_FreeChunk(speedup);
    Mix_FreeMusic(musica);
    Mix_CloseAudio();
    TTF_Quit();

    #ifdef PSP
	    sceKernelExitGame();
    #endif

    return 0;

}
