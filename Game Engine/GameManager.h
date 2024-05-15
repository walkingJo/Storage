#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameEngine.h"
#include "Object.h"

constexpr int MaxObjects = 50;

class GameManager {
public:
	GameManager() {
		engine = nullptr;
		objects = new Object * [MaxObjects];
		objSize = 0;
	}

	virtual void init() {
		return;
	}
	void setEngines(class GameEngine* engine) {
		this->engine = engine;
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

	//friend PhysicsEngine;
	//friend GraphicEngine;
public:
	Object** objects;
	int objSize;
};

#endif // !GAME_MANAGER_H