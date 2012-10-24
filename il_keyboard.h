#ifndef il_keyboard_h
#define il_keyboard_h
	#include "global.h"
	#include "il_screen.h"
	#include "il_player.h"
	#include "il_bullets.h"
	#include "enemygroup.h"
	#include "powerupsgroup.h"
	#include <SDL/SDL_mixer.h>

	// Classe respons�vel pela entrada de comandos do usu�rio
	class IL_Keyboard {

	public:

		// Variaveis para o controle das teclas do teclado. Ser� true quando a tecla respectiva estiver pressionada.
		bool leftPressed, rightPressed, upPressed, downPressed, spacePressed, F11pressed, pausePressed;
		EnemyGroup *groupDeEnemys;       // Referencia para que seja poss�vel reiniciar o jogo
		PowerUpsGroup *groupDePowerUps;       // Referencia para que seja poss�vel reiniciar o jogo
		IL_Bullets *bullets;       // Referencia para que seja poss�vel criar um novo bullet
		Timer *shootTimer;  // Timer para colocar um "delay" entre os intervalos da cria��o de um novo bullet
		IL_Screen *screen;  // referencia para a screen (para poder mudar para full screen e windowed)
		SDL_Joystick *stick;
		Mix_Chunk *laser;

		// Construtor - passe como parametro o(s) objetos que ser�o maniputaldos pelo teclado e crie um atributo novo para cada um deles
		IL_Keyboard(IL_Screen *screenNova, IL_Player *playerNovo, IL_Bullets *tirosNovo, EnemyGroup *groupDeEnemysNovo, PowerUpsGroup *groupDePowerUpsNovo, Mix_Chunk *laserNovo) {
		    player = playerNovo;
		    bullets = tirosNovo;
		    groupDeEnemys = groupDeEnemysNovo;
		    groupDePowerUps = groupDePowerUpsNovo;
		    leftPressed = false;
		    rightPressed = false;
		    upPressed = false;
		    downPressed = false;
		    spacePressed = false;
		    F11pressed = false;
		    pausePressed = false;
		    shootTimer = new Timer();
		    shootTimer->start();
		    screen = screenNova;
		    stick = SDL_JoystickOpen( 0);
		    laser = laserNovo;
		}

		~IL_Keyboard() {
		    SDL_JoystickClose( stick);
		}

		// Verifica se algum tecla foi pressionada ou solta.
		// Caso usuario tenha pressionado uma tecla, sua variavel respectiva torna-se true. Ficar� como false caso o usu�rio solte a tecla.
		void verificaTeclasPressionadas();

		// O m�todo verificaTeclasPressionadas() apenas seta as variaveis para true ou false.
		// O m�todo acoesDoTeclado � o respons�vel por executar de fato as a��es
		void acoesDoTeclado();

	private:

		IL_Player *player; // Referencia para o sprite do player, para que possamos movimenta-lo pelo teclado
		SDL_Event event;  // Objeto usado para capturar os eventos. Coloquei como atributo para n�o ficar instanciando toda hora.

	};
#endif
