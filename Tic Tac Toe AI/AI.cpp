#include "AI.h"

void AI::readFile() {
	return;
}
void AI::writeFile() {
	return;
}

bool AI::canSelect(short x, short y) {
	if (app->field[y][x] == DivideSign::NON)
		return true;
	else
		return false;
}

void AI::init(Game* app) {
	srand((unsigned int)time(NULL));
	this->app = app;
	readFile();
}
void AI::readyNewGame() {
	return;
}
Coord AI::selectSpace() {
	//short idxArray[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	//for (int i = 0; i < 9; ++i) {
	//	int rdnIdx = rand() % 9;
	//
	//	int temp = idxArray[i];
	//	idxArray[i] = idxArray[rdnIdx];
	//	idxArray[rdnIdx] = temp;
	//}
	//for (int i = 0; i < 9; ++i) {
	//	if (canSelect(i % 3, i / 3)) {
	//		app->wait();
	//		return Coord(i % 3, i / 3);
	//	}
	//}

	while (true) {
		short x = rand() % 3;
		short y = rand() % 3;
		if (canSelect(x, y)) {
			app->wait();
			return Coord(x, y);
		}
	}
}
void AI::release() {
	writeFile();
}
