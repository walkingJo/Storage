#pragma once
#ifndef GAME_H
#define GAME_H

#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>
#include "AI.h"

constexpr int ScreenWidth =  15 + 450 + 15;
constexpr int ScreenHeight = 60 + 450 + 60;

enum class DivideSign {
	COM,
	HUM,
	NON,
};

enum class MousePressType {
	COMMON,
	PRESSED,
	HOLDED,
	RELEASED,
};

class Game {
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	bool isRunning = true;
	short mouseXCoord = 0;
	short mouseYCoord = 0;
	//0 : ÀÏ¹Ý
	//1 : ´­¸²
	//2 : ¶¼Áü
	MousePressType mousePressType = MousePressType::COMMON;

	class AI* aiPlayer;
	//field
	DivideSign firstTurn = DivideSign::NON;
	DivideSign turn = DivideSign::NON;
	bool isButtonPressed = false;

	int humScore = 0;
	int comScore = 0;
	int drwScore = 0;

	void restartGame();
	DivideSign getWinner();
	bool isGameDone();
	void playerTurnUpdate();
	void aiTurnUpdate();
	
	void init();
	void input();
	void update();
	void render();
	void release();

public:
	DivideSign field[3][3] = {};

	void wait(int time);
	void wait();
	void run();

};

#endif // !GAME_H