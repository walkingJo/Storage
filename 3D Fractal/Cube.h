#pragma once

#include "raylib.h"

class Cube {
public:
	float size;
	Vector3 location;
	Color color;
	Cube(Vector3 srcVec, Color color) {
		this->size = 1.0f;
		this->location = srcVec;
		this->color = color;
	}
	Cube() {
		*this = Cube({ 0, 0, 0 }, { 0, 0, 0, 0 });
	}
};
