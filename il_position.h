#ifndef il_position_h
#define il_position_h
	// Representa uma posi��o X,Y.
	class IL_Position {

	public:

		// A posi��o � tratada como float para podermos colocar speeds n�o inteiras e esse type de coisa.
		// Mas na hora de exibir na tela, ser� feito um cast pra int.
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
