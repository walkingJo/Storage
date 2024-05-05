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
	void setEngines(class GameEngine* engine, class GraphicEngine* gEngine) {
		this->engine = engine;
		this->gEngine = gEngine;
	}
	virtual void setObjects() {
		return;
	}
	/* 실질적으로 게임을 돌아가게 하는 함수. */
	/* GameManager는 내부에 Object들의 목록을 가지고 있으며,
	*  GameManager::update()는 Object::update()를 포함한다.
	*/
	virtual void update(class InputProcessor* input) {
		return;
	}

private:
	GameEngine* engine;
	GraphicEngine* gEngine; //카메라 조작을 위한 필드다.
};

#endif // !GAME_MANAGER_H