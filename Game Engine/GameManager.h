#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameEngine.h"

class GameManager {
public:
	GameManager() {
		engine = nullptr;
		gEngine = nullptr;
	}

	virtual void init() {
		return;
	}
	void setEngine(class GameEngine* engine) {
		this->engine = engine;
	}
	void setEngines(class GameEngine* engine, class GraphicEngine* gEngine) {
		this->engine = engine;
		this->gEngine = gEngine;
	}
	/* ���������� ������ ���ư��� �ϴ� �Լ�. */
	virtual void update(class InputProcessor* input) {
		return;
	}

private:
	GameEngine* engine;
	GraphicEngine* gEngine;
};

#endif // !GAME_MANAGER_H