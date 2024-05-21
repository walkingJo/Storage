#pragma once
#ifndef AI_H
#define AI_H

#include <cstdlib>
#include <ctime>
#include "Game.h"

class Coord {
public:
	short x;
	short y;

	Coord() { x = y = 0; }
	Coord(short x, short y) {
		this->x = x;
		this->y = y;
	}
};

class AI {
private:
	const char* fileName = "playData.txt";

	class Game* app = nullptr;

	void readFile();
	void writeFile();

	//반추 함수 //reflect()
	//준비 함수 //함수를 만들지 말고 readyNewGame()에 넣자
	// -> readyNewGame()에 둘을 순서대로 포함시키도록 함

	bool canSelect(short x, short y);

public:
	void init(class Game* app);
	void readyNewGame();
	Coord selectSpace(); //하나의 수를 둔다는 표현이 뭐지?
	void release();

};

#endif // !AI_H