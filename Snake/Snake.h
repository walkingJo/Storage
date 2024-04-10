#pragma once

#include <cassert>
#include "Coord.h"

#pragma region <preprocessing>
#define FIELD_HOR	30 //40
#define FIELD_VER	15 //20
#define LENGTH_LIMIT (FIELD_HOR * FIELD_VER)
#pragma endregion

class Snake {
private:
	enum class Type { ALIVE, DEAD } type;
	Coord coords[LENGTH_LIMIT];
	int head = 0;
public:
	int length;
	Coord way;
	void addHead(Coord _head) {
		assert(length == 0);
		coords[head] = _head;
		length = 1;
	}
	void addTail() {
		coords[head + length] = coords[head + length - 1] - way;
		length++;
	}
	Snake(int _length) {
		assert(_length > 0);
		type = Type::ALIVE;
		head = 0;
		way = Coord(1, 0);

		length = 0;
		addHead(Coord(0, 0));
		for (int i = 1; i < _length; ++i)
			addTail();
	}
	Snake() { Snake(1); }
	// index 0 : head
	Coord operator[](int index) {
		return coords[(head + index) % LENGTH_LIMIT];
	}
	void move() {
		int oldHead = head;
		head = (LENGTH_LIMIT + head - 1) % LENGTH_LIMIT;
		coords[head] = coords[oldHead] + way;
	}
	void setALIVE() { type = Type::ALIVE; }
	void setDEAD() { type = Type::DEAD; }
	bool isAlive() { return (type == Type::ALIVE) ? true : false; }
	int getHead() { return head; }
};