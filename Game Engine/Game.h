#pragma once
#ifndef GAME_H
#define GAME_H

#include "GameEngine.h"
#include "GameManager.h"

class Game {
public:
	Game() {
		engine = new GameEngine();
		defaultManager = new GameManager();
	}
	~Game() {
		delete engine;
		delete defaultManager;
	}

	void run() {
		init();
		engine->run();
		this->~Game();
	}

private:
	GameEngine* engine;
	GameManager* defaultManager;

	virtual void init() {
		defaultManager->init();
		engine->init();
		engine->setManager(defaultManager);
	}
};

#endif // !GAME_H