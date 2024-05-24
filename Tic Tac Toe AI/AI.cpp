#include <fstream>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include "AI.h"

//AI::init() 에서만 호출됨
void AI::readFile() {
	std::ifstream fileReader(fileName);
	fileReader >> app->comScore;
	fileReader >> app->drwScore;
	fileReader >> app->humScore;
	for (int i = 0; i < TotalCaseCount; ++i) {
		fileReader >> cases[i].strCode;
		fileReader >> cases[i].weight;
	}
	fileReader.close();
}
//AI::release() 에서만 호출됨
void AI::writeFile() {
	std::ofstream fileWriter(fileName);
	fileWriter << app->comScore << ' ';
	fileWriter << app->drwScore << ' ';
	fileWriter << app->humScore << '\n';
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

Case AI::getCaseEqualsStr(string str) {
	for (Case singleCase : cases)
		if (str == singleCase.strCode)
			return singleCase;
	return cases[0];
}
void AI::setCaseEqualsStr(string str, float weight) {
	for (int i = 0; i < TotalCaseCount; ++i) {
		if (cases[i].strCode == str) {
			cases[i].weight = weight;
			break;
		}
	}
}
float AI::getACapB() {
	float totalWeight = 0.0f;
	for (string str : movesInSingleGame) {
		;
	}
	return 0.75;
}
void AI::analyzeWithGameResult(bool didAIWon) {
	/*
	* 베이즈 정리 : P(A|B) 를 통해 P(B|A) 를 구하는 정리
	*
	* P(A|B) = P(B|A) * P(A) / P(B)
	* P(B|A) = P(A|B) * P(B) / P(A)
	*
	* A : 수 t를 둘 확률
	* B : 승리할 확률
	* 라고 하자.
	*
	* P(B|A) = lastCase.weight 로 한다.
	* 그러면 새로운 A'에 대해
	*
	* A'=P(A|B) 가 된다.
	*/
	const Case& lastCase = getCaseEqualsStr(movesInSingleGame.back());
	float lastCaseWeight = lastCase.weight;

	if (didAIWon == true) {
		for (string moveStr : movesInSingleGame) {
			//float weight = getCaseEqualsStr(moveStr).weight;
			//float newWeight = getACapB() * lastCaseWeight / weight;
			
			float newWeight = getCaseEqualsStr(moveStr).weight + 0.1f;
			if (newWeight > 0.9f) newWeight = 0.9f;

			setCaseEqualsStr(moveStr, newWeight);
		}
	}
	else if (didAIWon == false) {
		for (string moveStr : movesInSingleGame) {
			//float newWeight = 0.75 * lastCaseWeight / getCaseEqualsStr(moveStr).weight;

			float newWeight = getCaseEqualsStr(moveStr).weight - 0.1f;
			if (newWeight < 0.1f) newWeight = 0.1f;

			setCaseEqualsStr(moveStr, newWeight);
		}
	}
}
void AI::analyze() {
	DivideSign winner = app->getWinner();
	switch (winner) {
	case DivideSign::COM: //AI 승
	case DivideSign::NON: //무승부
		analyzeWithGameResult(true);	break;
	case DivideSign::HUM: //AI 패
		analyzeWithGameResult(false);	break;
	}
}
Coord AI::getSingleDiffCoord(string srcStr, string dstStr) {
	//다른 좌표점(Coord)가 하나인지 확인하는 코드 추가
	for (short x = 0; x < 3; ++x)
		for (short y = 0; y < 3; ++y)
			if (srcStr[3 * y + x] != dstStr[3 * y + x] && canSelect(x, y))
				return Coord(x, y);
	return selectRandomCoord();
}
Coord AI::selectBestCoordWithRandom() {
	vector<Case> linkedCases = getLinkedCasesWith(fieldToStr());

	float weightSum = 0;
	for (Case singleCase : linkedCases) {
		if (singleCase.weight >= 0.9f)
			return getSingleDiffCoord(fieldToStr(), singleCase.strCode); //승률이 90% 이상이면 그 수를 선택하도록 함(사실상 승리 확정임 이거)
		weightSum += singleCase.weight;
	}
	float k = 1000 * 1.0f / weightSum;

	float randVal1000 = (float)(rand() % 999);
	for (int i = 0; i < linkedCases.size(); ++i) {
		if (randVal1000 > linkedCases[i].weight * k) {
			randVal1000 -= linkedCases[i].weight * k;
		}
		else
			getSingleDiffCoord(fieldToStr(), linkedCases[i].strCode);
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
