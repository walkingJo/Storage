#pragma once
#ifndef GAME_H
#define GAME_H

#include <ctime>
#include "RenderEngine.h"
#include "PlatformManager.h"
#include "InputProcessor.h"
#include "Player.h"

class Game {
private:
	RenderEngine renderer;
	PlatformManager platform;
	InputProcessor input;
	bool isRunning;

	Player playerObj;

	clock_t time;
	clock_t deltaTime;

	void init();
	void getInput();
	void update();
	void render();
	void wait();
	void release();

public:
	Game();
	void run();
};

#endif // !GAME_H