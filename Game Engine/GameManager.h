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
	/* ���������� ������ ���ư��� �ϴ� �Լ�. */
	/* GameManager�� ���ο� Object���� ����� ������ ������,
	*  GameManager::update()�� Object::update()�� �����Ѵ�.
	*/
	virtual void update(class InputProcessor* input) {
		return;
	}

private:
	GameEngine* engine;
	GraphicEngine* gEngine; //ī�޶� ������ ���� �ʵ��.
};

#endif // !GAME_MANAGER_H