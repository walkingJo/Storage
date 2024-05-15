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
	/* ���������� ������ ���ư��� �ϴ� �Լ�. */
	/* GameManager�� ���ο� Object���� ����� ������ ������,
	*  GameManager::update()�� Object::update()�� �����Ѵ�.
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