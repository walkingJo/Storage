#pragma once
#ifndef GAME_H
#define GAME_H

#include "RenderEngine.h"
#include "Object.h"

class Game {
public:
	Game();
	virtual ~Game();

	void run() {
		initialize();
		while (isRunning) {
			processInput();
			update();
		}
		release();
	}
private:
	void initialize();
	void fieldInit();
	void windowInit();

	void processInput();

	void update();
	void physicsUpdate();
	void renderUpdate();
	void timeUpdate();

	void release();

private:
	RenderEngine renderer;

	Object player;
	//std::vector<Object> objects;
	Object polygon1;
	Object polygon2;
	Object polygon3;

	Uint32 clock;
	float deltaTime;
	bool isRunning;
};

#endif // !GAME_H