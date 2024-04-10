#pragma once
class Coord {
public:
	int x;
	int y;
	Coord(int x, int y) : x(x), y(y) {  }
	Coord() { Coord(0, 0); }
	bool operator==(Coord other) {
		if ((this->x == other.x) && (this->y == other.y))
			return true;
		else
			return false;
	}
	bool operator!=(Coord other) {
		return !(this->operator==(other));
	}
	Coord operator+(Coord other) {
		Coord temp = *this;
		temp.x += other.x;
		temp.y += other.y;
		return temp;
	}
	Coord operator-(Coord other) {
		Coord temp = *this;
		temp.x -= other.x;
		temp.y -= other.y;
		return temp;
	}
};
