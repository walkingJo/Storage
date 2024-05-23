#pragma once
#ifndef AI_H
#define AI_H

#include <string>
#include <vector>
#include "Game.h"

constexpr int TotalCaseCount = 8953;

using std::string;
using std::vector;

class Case {
public:
	string strCode;
	float weight;

	Case() {
		strCode = "";
		weight = 0.5f;
	}
	Case(std::string code) {
		*this = Case();
		strCode = code;
	}
};

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

	Case cases[TotalCaseCount] = {};
	vector<string> movesInSingleGame;

	void readFile();
	void writeFile();

	string fieldToStr();
	bool isCaseLinked(string srcCase, string dstCase);
	vector<Case> getLinkedCasesWith(string nowCase);

	//반추 함수
	void analyze();
	Coord selectBestCoordWithRandom();
	Coord selectRandomCoord();

	bool canSelect(short x, short y);

public:
	void addMovement();

	void init(class Game* app);
	void readyNewGame(enum class DivideSign winner);
	Coord selectSpace(); //하나의 수를 둔다는 표현이 뭐지?
	void release();

};

#endif // !AI_H