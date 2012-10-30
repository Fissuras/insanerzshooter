#ifndef il_position_h
#define il_position_h

class IL_Position {

public:

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
