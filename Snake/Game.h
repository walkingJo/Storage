#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cstdarg>

#include <conio.h>
#include <time.h>
#include <Windows.h>
#include "Screen.h"

#pragma region <preprocessing>
#define FPS 15
#define ESC 27

#define START_TAIL_LEN	3
#define RESET_TAIL_MEN  30

#define SNAKE	'#'
#define APPLE	'@'
#define GROUND	' '
#pragma region <color>
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
#define RED			0x0C
#define PLUM		0x0D
#define YELLOW		0x0E
#define WHITE		0x0F
#pragma endregion
#pragma endregion

#pragma region <basic functions>
void gotoxy(int x, int y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void setColor(int color_num) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num);
}
char palette[128] = {};
void setColor_paletted(char character) {
	setColor(palette[character]);
}
void putc_palette(char character) {
	setColor(palette[character]);
	putchar(character);
}
#pragma endregion

class Game {
private:
	Snake snake;
	Screen oldScreen;
	Screen newScreen;
	Screen initScreen;

	bool isAppleExist;
	Coord apple;
	
	int highestScore = 0;
	clock_t deltaTime = 0;

	void printInfoBegintoEndLine() {
		printf("+");
		for (int i = 0; i < SCREEN_HOR - 2; ++i)
			printf("-");
		printf("+\n");
	}
	void printInfoLine(std::string text) {
		int textLen = SCREEN_HOR - 6 - text.size();
		assert(0 < textLen);
		printf("|  ");
		printf("%s", text.data());
		for (int i = 0; i < textLen; ++i)
			printf(" ");
		printf("  |\n");
	}
	/* divided by '|' */
	// '%' 로 불러와진 문자들은 색이 칠해지도록 변경
	void printfInfo(const char* format, ...) {
		va_list list;
		va_start(list, format);

		char formatedBuff[100] = {};
		vsprintf_s(formatedBuff, format, list);

		printInfoBegintoEndLine();
		char* buff = strtok(formatedBuff, "|");
		while (buff != NULL) {
			printInfoLine(buff);
			buff = strtok(NULL, "|");
		}
		printInfoBegintoEndLine();
	}
	void gamePause() {
		printfInfo("paused.|press %s to continue", "any key");
		_getch();
		gotoxy(0, SCREEN_VER);
		printf("                                        \n");
		printf("                                        \n");
		printf("                                        \n");
		printf("                                        \n");
	}
	void updateDrawSnake() {
		for (int i = 0; i < snake.length; ++i) {
			newScreen.addCoord(snake[i], SNAKE);
		}
	} // 필드에 뱀 그리기
	void addApple() {
		while (!isAppleExist) {
			apple = Coord(rand() % FIELD_HOR, rand() % FIELD_VER);
			if (newScreen.getCoordElement(apple) == GROUND) {
				isAppleExist = true;
				break;
			}
		}
		newScreen.addCoord(apple, APPLE);
	}
	void eatApple() {
		char fieldElement = newScreen.getCoordElement(snake[0]);
		if (fieldElement == APPLE) {
			isAppleExist = false;
			snake.length++;
			if (highestScore < snake.length)
				highestScore = snake.length;
		}
	}
	void updateApple() {
		addApple();
		eatApple();
	} // 사과 보충
	void updateCheckCollisionWall() {
		if (snake[0].x < 0)
			snake.setDEAD();
		else if (snake[0].y < 0)
			snake.setDEAD();
		else if (FIELD_HOR <= snake[0].x)
			snake.setDEAD();
		else if (FIELD_VER <= snake[0].y)
			snake.setDEAD();

		/*char fieldElement = newScreen.getCoordElement(snake[0]);
		switch (fieldElement) {
		case '+':
		case '-':
		case '|':
			snake.setDEAD();
		}*/
	} // 충돌 - 벽
	void updateCheckCollisionSelf() {
		for (int i = 1; i < snake.length; ++i) {
			if (snake[0] == snake[i])
				snake.setDEAD();
		}
	} // 충돌 - 자기 자신
	void Delay(int delayTime) {
		static clock_t nNewClock = 0;
		while (clock() < nNewClock + delayTime);
		deltaTime = clock() - nNewClock;
		nNewClock = clock();
	}
	void Delay() {
		Delay(1000 / FPS);
	}

	void Init() {
		system("cls");

		snake = Snake(START_TAIL_LEN);
		oldScreen = Screen('?');
		newScreen = Screen('?');

		isAppleExist = false;
		apple = Coord(0, 0);
	}
	void GetKey() {
		if (_kbhit()) {
			char key = _getch();
			if (key == 224) key = _getch();

			switch (key) {
			case ESC: case '0':
			case 'Q': case 'q':
				gamePause();
				break;

			case 'W': case 'w': case 72:
				if (snake.way != Coord(0, 1))
					snake.way = Coord(0, -1);
				break;
			case 'A': case 'a': case 75:
				if (snake.way != Coord(1, 0))
					snake.way = Coord(-1, 0);
				break;
			case 'S': case 's': case 80:
				if (snake.way != Coord(0, -1))
					snake.way = Coord(0, 1);
				break;
			case 'D': case 'd': case 77:
				if (snake.way != Coord(-1, 0))
					snake.way = Coord(1, 0);
				break;
			}
		}
	}
	void Update() {
		snake.move();
		oldScreen = newScreen;
		newScreen = initScreen;
		if (snake.length >= RESET_TAIL_MEN)
			snake.length = RESET_TAIL_MEN;

		updateDrawSnake();
		updateApple();
		updateCheckCollisionWall();
		updateCheckCollisionSelf();
	}
	void Render() {
		for (int x = 0; x < SCREEN_HOR; ++x) {
			for (int y = 0; y < SCREEN_VER; ++y) {
				if (oldScreen.getElement(y, x) != newScreen.getElement(y, x)) {
					gotoxy(x, y);
					putc_palette(newScreen.getElement(y, x));
				}
			}
		}
		gotoxy(2, SCREEN_VER - 2);
		setColor(0x07);
		printf("SCORE : %-4d  ", snake.length - START_TAIL_LEN);
		//printf("ΔTime : %-3.1fs", deltaTime / 1000.0f);
		gotoxy(0, SCREEN_VER);
	}
	bool GetRestart() {
		printfInfo("PRESS %s or %d to QUIT", "ESC", 0);
		gotoxy(5, SCREEN_VER - 2);

		Delay(1000);

		char key = _getch();
		switch (key) {
		case ESC:
		case '0':
			return false;
		default:
			return true;
		}
	}
	void Finish() {
		system("cls");
		printfInfo("highest score : %d", highestScore);

		Delay(1000);

		char key = _getch();
	}
public:
	Game() {
		system("cls");
		srand((unsigned int)time(NULL));

		palette['+'] = palette['-'] = palette['='] = palette['|'] = 0x0F;
		palette[SNAKE] = 0x0B;
		palette[APPLE] = 0x0C;

		initScreen = Screen(' ');
		for (int x = 0; x < SCREEN_HOR; ++x) {
			initScreen.field[0][x] =
				initScreen.field[SCREEN_VER - 3][x] =
				initScreen.field[SCREEN_VER - 1][x] = '-';
		}
		for (int y = 0; y < SCREEN_VER; ++y) {
			initScreen.field[y][0] =
				initScreen.field[y][SCREEN_HOR - 1] = '|';
			initScreen.field[y][SCREEN_HOR] = '\0';
		}
		initScreen.field[0][0] =
			initScreen.field[0][SCREEN_HOR - 1] =
			initScreen.field[SCREEN_VER - 3][0] =
			initScreen.field[SCREEN_VER - 3][SCREEN_HOR - 1] =
			initScreen.field[SCREEN_VER - 1][0] =
			initScreen.field[SCREEN_VER - 1][SCREEN_HOR - 1] = '+';
	}
	void run() {
		while (true) {
			Init();
			while (snake.isAlive()) {
				GetKey();
				Update();
				Render();
				Delay();
			}
			if (!GetRestart())
				break;
		}
		Finish();
	}
};
