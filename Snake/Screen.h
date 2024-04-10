#pragma once
#include "Snake.h"

#pragma region <preprocessing>
#define SCREEN_HOR (2 + FIELD_HOR)
#define SCREEN_VER (4 + FIELD_VER)
#pragma endregion

class Screen {
public:
	char field[SCREEN_VER][SCREEN_HOR + 1];
	Screen(char _default) {
		for (int x = 0; x <= SCREEN_HOR; ++x)
			for (int y = 0; y < SCREEN_VER; ++y)
				field[y][x] = _default;
	}
	Screen() { Screen(' '); }
	Screen operator=(Screen other) {
		for (int x = 0; x <= SCREEN_HOR; ++x) {
			for (int y = 0; y < SCREEN_VER; ++y) {
				this->field[y][x] = other.field[y][x];
			}
		}
		return *this;
	}
	void addCoord(Coord coord, char character) {
		field[coord.y + 1][coord.x + 1] = character;
	}
	char getElement(int i, int j) { return field[i][j]; }
	char getCoordElement(Coord coord) { return getElement(coord.y + 1, coord.x + 1); }
};