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

#include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>
#include <string>
#include <math.h>
#include <fstream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include "global.h"

#include "timer.h"
#include "il_position.h"
#include "il_sprite.h"

#include "bullet.h"
#include "il_bullets.h"

#include "particula.h"
#include "particlegroup.h"
#include "particlessystem.h"

#include "enemy.h"
#include "enemygroup.h"

#include "il_animatedtext.h"
#include "il_keyboard.h"
#include "il_player.h"
#include "il_screen.h"

#include "powerup.h"
#include "powerupsgroup.h"

// To compile it for GNU/Debian Linux and Ubuntu, please, replace "res/" with "/usr/share/insanerzshooter/"
// After that, run the createDebianPkg.sh script. The .deb package will be created. Have fun!

// Constantes de ambiente.. para poder compilar para o PSP e para PC
//#define psp // descomente esta linha para compilar para o PSP

#ifdef psp
    #include <pspkernel.h>
    #include <pspdebug.h>
    #include <pspsdk.h>
    #include <psppower.h>

    #ifdef PSPFW3X
    PSP_MODULE_INFO("InsanerzShooter", 0x0, 1, 1);
    PSP_HEAP_SIZE_KB(12*1024);
    #else
    PSP_MODULE_INFO("InsanerzShooter", 0x1000, 1, 1);
    #endif
#endif

#ifdef psp
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

void writetofile(char msg[], int tecla) {
    FILE* err = fopen("errorlog.txt", "a");
    fprintf(err, "%s %i\n", msg, tecla);
    fclose(err);
}

// Método criado para calcular o valor absoluto (ou seja, sem o sinal) de um número
// @param numero número a ser avaliado
double Abs(double number) {
    if (number >= 0 ) {
        return number;
    } else {
        return -number;
    }
}

int main(int argc, char *argv[]) {

    #ifdef psp
	    scePowerSetClockFrequency(333, 333, 166);
    	SetupCallbacks();
        atexit(sceKernelExitGame);
    #else
	    atexit(SDL_Quit);   // Avisa o sistema que antes de sair da aplicação o método SDL_QUIT deve ser executado
    #endif

    srand(time(NULL));// Melhora o sistema de geração de números randômicos (caso contrario a sequencia dos numeros gerados é sempre a mesma)
    char pontuacao[5];
    char *hiscoreChar;
    hiscoreChar = new char[5];
	int probDeCriarEnemy;// usado para calcular probabilidade de criar um novo enemy
    int systemTicks;
    int nextFrameTicks;

    FILE *pFile = fopen("hiscore.dat", "rt");
    if (pFile != NULL) {
        fgets(hiscoreChar, 6, pFile);
        HISCORE = strtol(hiscoreChar, NULL, 10);
        fclose(pFile);
    } else {
        HISCORE = 0;
    }

    IL_Screen *screen = new IL_Screen(false);// Cria uma nova tela
    SDL_ShowCursor(false);// Esconde o ponteiro do mouse
    SDL_WM_SetCaption("Insanerz Shooter", NULL);// Muda o título da janela
    TTF_Init();
    // iniciando sistema de som
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1200);
    Mix_AllocateChannels(MIX_DEFAULT_CHANNELS);

    // Carregando efeitos sonoros e musica
    Mix_Music *musica = Mix_LoadMUS ("res/musica.wav");
    Mix_Chunk *powerup = Mix_LoadWAV ("res/powerup.wav");
    Mix_Chunk *doubleshoot = Mix_LoadWAV ("res/doubleshot.wav");
    Mix_Chunk *tripleshoot = Mix_LoadWAV ("res/tripleshot.wav");
    Mix_Chunk *insaneshoot = Mix_LoadWAV ("res/insaneshot.wav");
    Mix_Chunk *laser = Mix_LoadWAV ("res/laser.wav");
    Mix_Chunk *explosion = Mix_LoadWAV ("res/explosion.wav");
    Mix_Chunk *playerExplosion = Mix_LoadWAV ("res/bomb.wav");
    Mix_Chunk *speedup = Mix_LoadWAV ("res/speedup.wav");

    Mix_PlayMusic(musica, -1);

     // Criando e posicionando o sprite do player (ignore o ultimo parametro, por enquanto não é usado)
    IL_Sprite playerSprite("res/player.png", 2);
    IL_Player *player = new IL_Player(playerSprite);

    // Criando e sprite do enemy (ignore o ultimo parametro, por enquanto não é usado)
    IL_Sprite spriteEnemy("res/enemy1.png", 2);
    IL_Sprite spriteEnemy2("res/enemy2.png", 2);
    IL_Sprite spriteEnemy3("res/enemy3.png", 2);

    // Cria um backgroundGroup inicial de enemies
    EnemyGroup groupDeEnemys = EnemyGroup();

    // Cria um backgroundGroup inicial de powerups vazio
    PowerUpsGroup groupDePowerUps = PowerUpsGroup(player);
    IL_Sprite powerUp1Sprite("res/powerup1.png", 2);
    IL_Sprite powerUp2Sprite("res/powerup2.png", 2);

	// Cria objeto responsável por controlar os bullets
    IL_Bullets *bullets = new IL_Bullets();

	// Cria objeto responsável por controlar o teclado
    IL_Keyboard teclado = IL_Keyboard(screen, player, bullets, &groupDeEnemys, &groupDePowerUps, laser);

    // Carrega a fonte em 2 tamanhos diferentes
    TTF_Font *font = TTF_OpenFont("res/FreeSans_bold.ttf", 20);
    TTF_Font *fontPequena = TTF_OpenFont("res/FreeSans_bold.ttf", 16);

    // CORES
    SDL_Color corBranca = {255, 255, 255};
    SDL_Color corAmarela = {255, 255, 0};

    // Cria uma surface usada para exibir SCORE
    SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, "SCORE", corAmarela);
    SDL_Rect scoreRect;
    scoreRect.x = 20;
    scoreRect.y = 0;
    // Cria uma surface e um SDL_Rect (posicao) usados para exibir a pontuação do player
    SDL_Surface *playerScoreSurface = TTF_RenderText_Solid(font, "0", corBranca);
    SDL_Rect *playerScoreRect = new SDL_Rect();
    playerScoreRect->x = 20;
    playerScoreRect->y = 20;

    // Cria uma surface usada para exibir HISCORE
    SDL_Surface *hiscoreSurface = TTF_RenderText_Solid(font, "HISCORE", corAmarela);
    SDL_Rect hiscoreRect;
    hiscoreRect.x = SCREEN_WIDTH - hiscoreSurface->w - 20;
    hiscoreRect.y = 0;
    // Cria uma surface e um SDL_Rect (posicao) usados para exibir a pontuação mais alta do player
    SDL_Surface *hiscoreNUMSurface = TTF_RenderText_Solid(font, "0", corBranca);
    SDL_Rect *hiscoreNUMRect = new SDL_Rect();
    hiscoreNUMRect->x = SCREEN_WIDTH - hiscoreSurface->w - 20;
    hiscoreNUMRect->y = 20;

    // Cria uma surface usada para exibir PAUSE
    SDL_Surface *pauseSurface = TTF_RenderText_Solid(font, "P A U S E", corBranca);
    SDL_Rect pauseRect;
    pauseRect.x = (SCREEN_WIDTH - pauseSurface->w) / 2;
    pauseRect.y = (SCREEN_HEIGHT - pauseSurface->h) / 2;

    // Cria uma surface e um SDL_Rect (posicao) usados para exibir a energia da gun do player
    SDL_Surface *bulletSurface = TTF_RenderText_Solid(font, "BULLETS", corAmarela);
    SDL_Rect bulletRect;
    bulletRect.x = SCREEN_WIDTH * 0.4;
    bulletRect.y = 20;

    // Cria uma surface usada para exibir a URL
    SDL_Surface *urlSurface = TTF_RenderText_Solid(fontPequena, "http://insanerzshooter.googlepages.com", corAmarela);
    SDL_Rect urlRect;
    urlRect.x = 0;
    urlRect.y = 0;

	// Cria uma surface usada para exibir o início
    SDL_Surface *pressFireSurface = TTF_RenderText_Solid(fontPequena, "Press FIRE to start", corAmarela);
    SDL_Rect pressFireRect;
    pressFireRect.x = (SCREEN_WIDTH - pressFireSurface->w) / 2;
    pressFireRect.y = int(SCREEN_HEIGHT - (2 * 16));

    // Logo Insanerz Shooter (parte do insanerz)
    IL_AnimatedText insanerz[8];
    insanerz[0] = IL_AnimatedText("I", 0,   0, false, font);
    insanerz[1] = IL_AnimatedText("N", 30,  20, false, font);
    insanerz[2] = IL_AnimatedText("S", 60,  40, false, font);
    insanerz[3] = IL_AnimatedText("A", 90,  60, false, font);
    insanerz[4] = IL_AnimatedText("N", 120, 40, false, font);
    insanerz[5] = IL_AnimatedText("E", 150, 20, false, font);
    insanerz[6] = IL_AnimatedText("R", 180, 0, false, font);
    insanerz[7] = IL_AnimatedText("Z", 210, 20, false, font);
    SDL_Rect insanerzRect;

    // Logo Insanerz Shooter (parte do shooter)
    IL_AnimatedText shooter[7];
    shooter[0] = IL_AnimatedText("S", 0,   0, false, font);
    shooter[1] = IL_AnimatedText("H", 30,  20, false, font);
    shooter[2] = IL_AnimatedText("O", 60,  40, false, font);
    shooter[3] = IL_AnimatedText("O", 90,  60, false, font);
    shooter[4] = IL_AnimatedText("T", 120, 40, false, font);
    shooter[5] = IL_AnimatedText("E", 150, 20, false, font);
    shooter[6] = IL_AnimatedText("R", 180, 0, false, font);

    ParticleGroup *backgroundGroup = new ParticleGroup();
    for (int i=0; i < NUMBER_OF_BACKGROUND_STARS; i++) {
        backgroundGroup->addParticleSystem(rand()%SCREEN_WIDTH, rand()%SCREEN_HEIGHT, 2, 0, -1);
    }

    while (true) {

        systemTicks = SDL_GetTicks();
        nextFrameTicks = systemTicks + 8;

        screen->limpar();// Pinta tudo de preto
        backgroundGroup->draw(screen->surface);// desenha as particulas
        screen->drawBarraFirePower(player);// Desenha a parte superior da tela
        groupDePowerUps.actAndDraw(screen->surface);
        groupDeEnemys.checkCollision(bullets, backgroundGroup, explosion);// Verifica se player matou algum enemy
        groupDeEnemys.actAndDraw(screen->surface, backgroundGroup);// Move os enemies e desenha eles na tela

        if (!GAME_PAUSED) {
            bullets->updatePositions();// Move os bullets
        }

        bullets->draw(screen->surface);// Desenha bullets na tela
        teclado.verificaTeclasPressionadas();// Verifica quais teclas estão sendo pressionadas

        if (PLAYER_ALIVE) {
            groupDePowerUps.checkCollision(powerup, doubleshoot, tripleshoot, insaneshoot, speedup);
            screen->draw(player->playerSprite);// Desenha player na tela
            // probabilidade de criar particula de "fogo" da nave
            int probDeCriarParticulaDeFogo = 6 - ((int) player->speed);// a prob de criar particula de fogo será proporcional a vel da nave
            if (!GAME_PAUSED) {
                if (rand()%probDeCriarParticulaDeFogo == 1) {
                    backgroundGroup->addParticleSystem(player->playerSprite.position.x, player->playerSprite.position.y, 1, 0, 40);
                }
            } else {
                SDL_BlitSurface(pauseSurface, NULL, screen->surface, &pauseRect);// Exibe nome do jogo
            }
        } else {
            // Desenhando nome do jogo
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
                SDL_BlitSurface(pressFireSurface, NULL, screen->surface, &pressFireRect);// Exibe "Press FIRE to start"
            }

            SDL_BlitSurface(urlSurface, NULL, screen->surface, &urlRect);// Exibe a URL

        }

        teclado.acoesDoTeclado();// Realiza ações do teclado (move a nave, atira, etc)

		//Exibe o HEADER
        if (PLAYER_ALIVE) {
            SDL_BlitSurface(scoreSurface, NULL, screen->surface, &scoreRect);
            SDL_BlitSurface(bulletSurface, NULL, screen->surface, &bulletRect);
        }
        SDL_BlitSurface(hiscoreSurface, NULL, screen->surface, &hiscoreRect);

		// Verifica se o player foi atingido
        if (PLAYER_ALIVE == true && groupDeEnemys.checkCollision(player)) {
            Mix_PlayChannel(-1, playerExplosion, 0);
            player->deathTimer->start();
            PLAYER_ALIVE = false;
            if (PLAYER_SCORE > HISCORE) {
                HISCORE = PLAYER_SCORE;
				// Adiciona a pontuacao maxima no arquivo de highscore
                FILE *pFile = fopen("hiscore.dat", "w+");
                sprintf(hiscoreChar,"%i",HISCORE);
                if (pFile != NULL) {
                    fputs(hiscoreChar, pFile);
                    fclose(pFile);
                }
            }
            backgroundGroup->addParticleSystem(player->playerSprite.position.x, player->playerSprite.position.y, 0, 500, 400);
        }

		// Exibe a pontuacao
        if (PLAYER_ALIVE) {
            sprintf(pontuacao,"%i",PLAYER_SCORE);
            #ifdef psp
            	playerScoreSurface = TTF_RenderText_Solid(fontPequena, pontuacao, corBranca);
            #else
            	playerScoreSurface = TTF_RenderText_Solid(font, pontuacao, corBranca);
            #endif
			playerScoreRect->x = SCREEN_WIDTH * 0.05 + ((scoreSurface->w - playerScoreSurface->w) / 2);
            SDL_BlitSurface(playerScoreSurface, NULL, screen->surface, playerScoreRect);
            SDL_FreeSurface(playerScoreSurface);
        }

		// Exibe a pontuacao maxima
        sprintf(hiscoreChar,"%i",HISCORE);
        #ifdef psp
    	    hiscoreNUMSurface = TTF_RenderText_Solid(fontPequena, hiscoreChar, corBranca);
        #else
	        hiscoreNUMSurface = TTF_RenderText_Solid(font, hiscoreChar, corBranca);
        #endif
		hiscoreNUMRect->x = (SCREEN_WIDTH * 0.75) + ((hiscoreSurface->w - hiscoreNUMRect->w) / 2);
        SDL_BlitSurface(hiscoreNUMSurface, NULL, screen->surface, hiscoreNUMRect);
        SDL_FreeSurface(hiscoreNUMSurface);

        // Se necessário, aguarda alguns milisegundos para manter o FPS constante
        systemTicks = SDL_GetTicks();
        if (nextFrameTicks > systemTicks) SDL_Delay(nextFrameTicks-systemTicks);
        // Exibe o novo cenário ao usuário
        SDL_Flip(screen->surface);

        // se tiver menos do que 50 enemies, tem certa probabilidade de criar um novo enemy
        if (groupDeEnemys.enemies.size() < 50 && !GAME_PAUSED) {
            if (PLAYER_SCORE < 100 && groupDeEnemys.enemies.size() < 30) {
                probDeCriarEnemy = rand() % 100;
            } else if (PLAYER_SCORE < 300 && groupDeEnemys.enemies.size() < 30) {
                probDeCriarEnemy = rand() % 90;
            } else if (PLAYER_SCORE < 500 && groupDeEnemys.enemies.size() < 30) {
                probDeCriarEnemy = rand() % 70;
            } else if (PLAYER_SCORE < 1000 && groupDeEnemys.enemies.size() < 30) {
                probDeCriarEnemy = rand() % 50;
            } else if (PLAYER_SCORE < 2000 && groupDeEnemys.enemies.size() < 35) {
                probDeCriarEnemy = rand() % 30;
            } else if (PLAYER_SCORE < 3000 && groupDeEnemys.enemies.size() < 40) {
                probDeCriarEnemy = rand() % 30;
            } else if (PLAYER_SCORE < 4000 && groupDeEnemys.enemies.size() < 45) {
                probDeCriarEnemy = rand() % 25;
            } else if (groupDeEnemys.enemies.size() < 50) {
                probDeCriarEnemy = rand() % 10;
            }

            if (probDeCriarEnemy == 1) {
                // 0 = apenas anda para baixo
                // 1 = zigue zague para direita
                // 2 = zigue zague para esquerda
                // 3 = giro curto sentido horario em seguida anda reto para baixo
                // 4 = giro curto sentido anti-horario
                // 5 = giro curto sentido horario
                // 6 = giro longo sentido horario
                // 7 = giro longo sentido anti-horario
                if (PLAYER_SCORE > 30) {
                    int typeRand = rand()%4;
                    if (typeRand == 0) {
                        groupDeEnemys.createNewEnemy(spriteEnemy3, typeRand);
                    } else {
                        groupDeEnemys.createNewEnemy(spriteEnemy, typeRand);
                    }
                } else {
                    int typeRand = rand()%3;
                    if (typeRand == 0) {
                        groupDeEnemys.createNewEnemy(spriteEnemy3, typeRand);
                    } else {
                        groupDeEnemys.createNewEnemy(spriteEnemy, typeRand);
                    }
                }

            } else if (probDeCriarEnemy == 2) {
                if (PLAYER_SCORE > 30 && PLAYER_SCORE < 150) {
                    if (rand()%8 == 1) {
                        groupDeEnemys.createNewEnemy(spriteEnemy2, 4 + rand()%2);
                    }
                } else if (PLAYER_SCORE > 150) {
                    groupDeEnemys.createNewEnemy(spriteEnemy2, 4 + rand()%4);
                }
            }
        }

        // Probabilidade de criar um powerup novo
        int probDeCriarPowerUp = rand() % 3000;
		if (!GAME_PAUSED) {
			if (probDeCriarPowerUp == 1) {
				PowerUp p = PowerUp(powerUp1Sprite, 0);
				groupDePowerUps.add(p);
			} else if (probDeCriarPowerUp == 2) {
				if (player->speed < 4) {
					PowerUp p = PowerUp(powerUp2Sprite, 1);
					groupDePowerUps.add(p);
				} else {
					PowerUp p = PowerUp(powerUp1Sprite, 0);
					groupDePowerUps.add(p);
				}
			}
		}
    }

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

    #ifdef psp
	    sceKernelExitGame();
    #endif

    return 0;

}
