#include <fstream>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include "AI.h"

//AI::init() 에서만 호출됨
void AI::readFile() {
	std::ifstream fileReader(fileName);
	for (int i = 0; i < TotalCaseCount; ++i) {
		fileReader >> cases[i].strCode;
		fileReader >> cases[i].weight;
	}
	fileReader.close();
}
//AI::release() 에서만 호출됨
void AI::writeFile() {
	std::ofstream fileWriter(fileName);
	for (int i = 0; i < TotalCaseCount; ++i) {
		fileWriter << cases[i].strCode;
		fileWriter << ' ';
		fileWriter << cases[i].weight;
		fileWriter << '\n';
	}
	fileWriter.close();
}

string AI::fieldToStr() {
	string str = "_________";
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			switch (app->field[y][x]) {
			case DivideSign::COM: str[3 * y + x] = 'X'; break;
			case DivideSign::HUM: str[3 * y + x] = 'O'; break;
			case DivideSign::NON: break;
			}
		}
	}

	return str;
}
bool AI::isCaseLinked(string srcCase, string dstCase) {
	short srcRank = 0, dstRank = 0;
	for (int i = 0; i < 9; ++i) {
		if (srcCase[i] != '_') srcRank++;
		if (dstCase[i] != '_') dstRank++;
	}
	if (srcRank + 1 != dstRank)
		return false;

	short diffCount = 0;
	short onlyOXDiffCount = 0;
	for (int i = 0; i < 9; ++i) {
		if (srcCase[i] != dstCase[i]) {
			diffCount++;
			if (srcCase[i] != '_' &&
				dstCase[i] != '_')
				onlyOXDiffCount++;
		}
	}
	if (diffCount == 1 && onlyOXDiffCount == 0)
		return true;
	else
		return false;
}
vector<Case> AI::getLinkedCasesWith(string nowCase) {
	vector<Case> linkedCases = vector<Case>();

	for (int i = 0; i < TotalCaseCount; ++i) {
		if (isCaseLinked(nowCase, cases[i].strCode))
			linkedCases.emplace_back(cases[i]);
	}
	return linkedCases;
}

void AI::analyze() {
	/*
	* 베이즈 정리 : P(A|B) 를 통해 P(B|A) 를 구하는 정리
	* 
	* P(A|B) = P(B|A) * P(A) / P(B)
	* 
	* A : 승리할 확률		: lastCase.weight
	* B : 수 t를 둘 확률	: case.weight 에 가공을 거친 것
	* 라고 하자.
	*/

	DivideSign winner = app->getWinner();
	switch (winner) {
	case DivideSign::COM: //AI 승
		break;
	case DivideSign::HUM: //AI 패
		break;
	case DivideSign::NON: //무승부
		break;
	}
}
Coord AI::selectBestCoordWithRandom() {
	vector<Case> linkedCases = getLinkedCasesWith(fieldToStr());

	float weightSum = 0;
	for (Case singleCase : linkedCases)
		weightSum += singleCase.weight;
	float k = 1000 * 1.0f / weightSum;

	float randVal1000 = (float)(rand() % 999);
	for (int i = 0; i < linkedCases.size(); ++i) {
		if (randVal1000 > linkedCases[i].weight * k) {
			randVal1000 -= linkedCases[i].weight * k;
		}
		else {
			for (short x = 0; x < 3; ++x)
				for (short y = 0; y < 3; ++y)
					if (fieldToStr()[3 * y + x] != linkedCases[i].strCode[3 * y + x] && canSelect(x, y))
						return Coord(x, y);
		}
	}
	return selectRandomCoord();
}
Coord AI::selectRandomCoord() {
	while (true) {
		short x = rand() % 3;
		short y = rand() % 3;
		if (canSelect(x, y))
			return Coord(x, y);
	}
}

bool AI::canSelect(short x, short y) {
	if (app->field[y][x] == DivideSign::NON)
		return true;
	else
		return false;
}

void AI::addMovement() {
	string str = fieldToStr();
	movesInSingleGame.emplace_back(str);
}

void AI::init(Game* app) {
	srand((unsigned int)time(NULL));
	this->app = app;

	for (int i = 0; i < TotalCaseCount; ++i) {
		cases[i] = Case();
	}
	movesInSingleGame = vector<string>();

	readFile();
}
//Game::restartGame()에서만 호출됨
void AI::readyNewGame(enum class DivideSign winner) {
	//movesInSingleGame & moveCount의 내용을 건들여야
	if (winner == DivideSign::HUM || winner == DivideSign::COM)
		analyze();

	/*for (string str : movesInSingleGame) {
		for (int i = 0; i < 9; ++i) {
			if (i % 3 == 0)
				printf("\n");
			printf("%c", str[i]);
		}
		printf("\n");
	}
	printf("\n");
	printf("############\n");
	printf("\n");*/

	movesInSingleGame = vector<string>();
}
Coord AI::selectSpace() {
	app->wait();

	/* AI의 본분을 다하는 코드 */
	return selectBestCoordWithRandom();

	/* random method */
	return selectRandomCoord();
}
void AI::release() {
	writeFile();
}
