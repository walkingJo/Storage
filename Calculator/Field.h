#pragma once
#ifndef FIELD_H
#define FIELD_H

#include <Windows.h>
#include <stdio.h>
#include "Calculator.h"
#include "ConsolePalette.h"
#include "Coord.h"

void gotoxy(short x, short y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

class Field {
private:
	char textBox[12][32] = {};
	unsigned char colorBox[12][32] = {};
public:
	Field() {
		reset();
	}
	void reset() {
		const char* calcFrame[12] = {
			"+-----------------------------+",
			"|                             |",
			"|                             |",
			"|                             |",
			"+-----+-----+-----+-----+-----+",
			"|     |     |     |     |     |",
			"|     |     |     |     |     |",
			"|     |     |     |     |     |",
			"|     |     |     |     |     |",
			"|     |     |     |     |     |",
			"|     |     |     |     |     |",
			"+-----+-----+-----+-----+-----+"
		};
		const char* buttons[6][5] = {
			{ "x^2","pi "," e ","cls","bsp" },
			{ "x^y"," ( "," ) "," ! "," / " },
			{ " ? "," 7 "," 8 "," 9 "," * " },
			{ " ? "," 4 "," 5 "," 6 "," - " },
			{ "log"," 1 "," 2 "," 3 "," + " },
			{ "ln ","+/-"," 0 "," . "," = " }
		};
		for (int row = 0; row < 12; ++row) {
			for (int col = 0; col < 32; ++col) {
				textBox[row][col] = calcFrame[row][col];
				colorBox[row][col] = Color::BLACK * 0x10 + Color::ORIGINAL;
			}
		}
		for (int bRow = 0; bRow < 6; ++bRow) {
			for (int bCol = 0; bCol < 5; ++bCol) {
				textBox[bRow + 5][bCol * 6 + 2] = buttons[bRow][bCol][0];
				textBox[bRow + 5][bCol * 6 + 3] = buttons[bRow][bCol][1];
				textBox[bRow + 5][bCol * 6 + 4] = buttons[bRow][bCol][2];
			}
		}
	}
	void setExpression(string expression) {
		for (int i = 0; i < expression.length(); ++i)
			textBox[2][2 + i] = expression[i];
	}
	void setResult(string result) {
		int strlen = result.length();
		//가장 마지막 문자가 (28, 3)에 위치해야 함
		for (int x = 0; x < strlen; ++x)
			textBox[3][x + 29 - strlen] = result[x];
	}
	void print(Coord cursor) {
		system("cls");
		setCursorColor(cursor);
		for (int row = 0; row < 12; ++row)
			for (int col = 0; col < 32; ++col) {
					gotoxy(col, row);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorBox[row][col]);
					printf("%c", textBox[row][col]);
				}
		gotoxy(31, 11);
	}
	void print(Coord cursor, Field oldBufferField) {
		setCursorColor(cursor);
		for (int row = 0; row < 12; ++row)
			for (int col = 0; col < 32; ++col)
				if (textBox[row][col] != oldBufferField.textBox[row][col] ||
					colorBox[row][col] != oldBufferField.colorBox[row][col]) {
					gotoxy(col, row);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorBox[row][col]);
					printf("%c", textBox[row][col]);
				}
		gotoxy(31, 11);
	}
	void setCursorColor(Coord cursor, unsigned char color = Color::SKY_BLUE * 0x10 + Color::GOLD) {	
		int row = cursor.y + 5;
		for (int col = cursor.x * 6 + 1; col <= cursor.x * 6 + 5; ++col)
			colorBox[row][col] = color;
	}
};

#endif // !FIELD_H