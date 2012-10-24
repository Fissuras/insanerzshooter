#ifndef il_position_h
#define il_position_h
	// Representa uma posição X,Y.
	class IL_Position {

	public:

		// A posição é tratada como float para podermos colocar speeds não inteiras e esse type de coisa.
		// Mas na hora de exibir na tela, será feito um cast pra int.
		float x, y;

		IL_Position() {
		    x = 0;
		    y = 0;
		}

		IL_Position(int newX, int newY) {
		    x = newX;
		    y = newY;
		}

		~IL_Position() {}

	};
#endif
