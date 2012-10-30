#include "il_animatedtext.h"

void IL_AnimatedText::animate() {
	varAuxiliarCos = varAuxiliarCos - 0.3;
	if (varAuxiliarCos < 0) {
		varAuxiliarCos = 0;
	}

	if (estaSubindo) {
		posicaoChar.y = posicaoChar.y - cos(varAuxiliarCos);
	} else {
		posicaoChar.y = posicaoChar.y + cos(varAuxiliarCos);
	}

	if (posicaoChar.y <= 0 && estaSubindo) {
		estaSubindo = false;
		varAuxiliarCos = 1;
	} else if (posicaoChar.y > 60 && !estaSubindo) {
		estaSubindo = true;
		varAuxiliarCos = 1;
	}

}
