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

	//���� �Լ� //reflect()
	//�غ� �Լ� //�Լ��� ������ ���� readyNewGame()�� ����
	// -> readyNewGame()�� ���� ������� ���Խ�Ű���� ��

	bool canSelect(short x, short y);

public:
	void init(class Game* app);
	void readyNewGame();
	Coord selectSpace(); //�ϳ��� ���� �дٴ� ǥ���� ����?
	void release();

};

#endif // !AI_H