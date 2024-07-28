//----------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------- coord      --------------------------------------

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
	bool operator!=(Coord other) { return !(*this == other); }

	Coord operator+(Coord other) { return Coord(this->x + other.x, this->y + other.y); }
	Coord operator-(Coord other) { return Coord(this->x - other.x, this->y - other.y); }
	Coord operator*(int other) { return Coord(this->x * other, this->y * other); }
	Coord operator/(int other) { return Coord(this->x / other, this->y / other); }
	Coord operator%(int other) { return Coord(this->x % other, this->y % other); }

	Coord operator=(Coord other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}
	Coord operator+=(Coord other) { return *this = *this + other; }
	Coord operator-=(Coord other) { return *this = *this - other; }
	Coord operator*=(int other) { return *this = *this * other; }
	Coord operator/=(int other) { return *this = *this / other; }
	Coord operator%=(int other) { return *this = *this % other; }
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------- including header files     ----------------------

/* programFile.h */
#ifndef PROGRAM_FILE_H
#define PROGRAM_FILE_H

/* ... header file code ... */

#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------- goto (x, y)        ------------------------------

#include <Windows.h>

void gotoxy(int x, int y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------- is number(only integer)    ----------------------

#include <cassert>

bool isNumber(char text) {
	assert(32 <= text && text <= 126);
	if ('0' <= text && text <= '9')
		return true;
	else
		return false;
}

bool isNumber(const char* text) {
	for (int i = 0; i < strlen(text); ++i)
		if (!isNumber(text[i]))
			return false;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------- pixel color        ------------------------------

#define BLACK		0x00
#define DARK_BLUE	0x01
#define GREEN		0x02
#define BLUE_GREEN	0x03
#define BLOOD		0x04
#define PURPLE		0x05
#define GOLD		0x06
#define ORIGINAL	0x07
#define GRAY		0x08
#define BLUE		0x09
#define HIGH_GREEN	0x0A
#define SKY_BLUE	0x0B
#define RED		0x0C
#define PLUM		0x0D
#define YELLOW		0x0E
#define WHITE		0x0F

void setConsoleColor(int color_num) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num);
}

char palette[128] = {};

void setColor_paletted(char character) {
	setColor(palette[character]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
