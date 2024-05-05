#pragma once
#ifndef GAME_H
#define GAME_H

#include "GameEngine.h"
#include "GameManager.h"

class Game {
public:
	Game() {
		engine = new GameEngine();
		manager = new GameManager();
	}
	~Game() {
		delete engine;
		delete manager;
	}

	void run() {
		init();
		engine->run();
		this->~Game();
	}

private:
	GameEngine* engine;
	GameManager* manager;

	virtual void init() {
		manager->init();
		engine->init();
		engine->setManager(manager);
	}
};

#endif // !GAME_H