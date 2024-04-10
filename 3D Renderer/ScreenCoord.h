#pragma once
#ifndef SCREEN_COORD_H
#define SCREEN_COORD_H

#include <SDL.h>

class ScreenCoord {
public:
	int x;
	int y;
	ScreenCoord(int x, int y) : x(x), y(y) {  }
	ScreenCoord() { ScreenCoord(0, 0); }
	bool operator==(ScreenCoord other) {
		if ((this->x == other.x) && (this->y == other.y))
			return true;
		else
			return false;
	}
	bool operator!=(ScreenCoord other) { return !(*this == other); }

	ScreenCoord operator+(ScreenCoord other) { return ScreenCoord(this->x + other.x, this->y + other.y); }
	ScreenCoord operator-(ScreenCoord other) { return ScreenCoord(this->x - other.x, this->y - other.y); }
	ScreenCoord operator*(int other) { return ScreenCoord(this->x * other, this->y * other); }
	ScreenCoord operator/(int other) { return ScreenCoord(this->x / other, this->y / other); }
	ScreenCoord operator%(int other) { return ScreenCoord(this->x % other, this->y % other); }

	ScreenCoord operator=(ScreenCoord other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}
	ScreenCoord operator+=(ScreenCoord other) { return *this = *this + other; }
	ScreenCoord operator-=(ScreenCoord other) { return *this = *this - other; }
	ScreenCoord operator*=(int other) { return *this = *this * other; }
	ScreenCoord operator/=(int other) { return *this = *this / other; }
	ScreenCoord operator%=(int other) { return *this = *this * other; }

	// 수정 필요
	SDL_Vertex toVertex() {
		SDL_Vertex temp = { x, y };
		return temp;
	}
};

#endif // !SCREEN_COORD_H