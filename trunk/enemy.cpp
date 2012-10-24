#include "enemy.h"
#include <math.h>

// Método responsável pelo comportamento do enemy.
// Cada type de enemy tem um comportamento.
// Talvez fosse mais interessante usar polimorfismo e dividir em várias classes os enemies, mas tive problemas para fazer isso em C++.
void Enemy::act() {

	if (GAME_PAUSED) {
		return;
	}

	if (type == 0) {// 0 = apenas anda para baixo
		position.y = position.y + speed;

	} else if (type == 1) {// 1 = zigue zague para direita
		position.y = position.y + speed;
		position.x = position.x + (sin(seno)) + 1;
		seno = seno + 0.1;
		if (position.x + 29 > SCREEN_WIDTH) {
			type = 2;
		}

	} else if (type == 2) {// 2 = zigue zague para esquerda
		position.y = position.y + speed;
		position.x = position.x + (sin(seno)) - 1;
		seno = seno + 0.1;
		if (position.x < 0) {
			type = 1;
		}

	} else if (type == 3) {// 3 = giro curto sentido horario em seguida anda reto para baixo
		if (position.y < 200) {
			position.x = position.x + (cos(seno) * 5);
			position.y = position.y + (sin(seno) * 5) + speed;
			seno = seno + 0.1;
		} else {
			position.y = position.y + speed;
		}

	} else if (type == 4) {// 4 = giro curto sentido anti-horario
		position.x = position.x - (cos(seno) * 5);
		position.y = position.y + (sin(seno) * 5) + speed;
		seno = seno + 0.1;

	} else if (type == 5) {// 5 = giro curto sentido horario
		position.x = position.x + (cos(seno) * 5);
		position.y = position.y + (sin(seno) * 5) + speed;
		seno = seno + 0.1;

	} else if (type == 6) {// 6 = giro longo sentido horario
		position.x = position.x + (cos(seno) * 5);
		position.y = position.y + (sin(seno) * 5) + speed;
		seno = seno + 0.03;

	} else if (type == 7) {// 7 = giro longo sentido anti-horario
		position.x = position.x - (cos(seno) * 5);
		position.y = position.y + (sin(seno) * 5) + speed;
		seno = seno + 0.03;
	}

	// Se sair da tela, volta para o topo em uma posição X aleatória
	if ( position.y > int(SCREEN_WIDTH - (SCREEN_WIDTH * 0.125)) ) {
		position.y = -250;
		position.x = rand() % int(SCREEN_WIDTH - (SCREEN_WIDTH * 0.125));
		seno = 0;
	}

}
