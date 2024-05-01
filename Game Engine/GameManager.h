#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameEngine.h"

class GameManager {
public:
	GameManager() {
		engine = nullptr;
	}

	virtual void init() {
		//
	}
	void setEngine(class GameEngine* engine) {
		this->engine = engine;
	}
	//���� ������ ������ ������ �����ϴ� �ڵ带 ������Ʈ�� �ֵ���.
	//��¼�� GameManager�� �ټ� �����ؼ� Ư�� ��Ȳ���� ��ü�� ����ϴ� �͵� �����ҵ�
	//    >> ��ġ Scene�� ���� Ȱ��
	virtual void update() {
		return;
	}
private:
	class GameEngine* engine;
};

#endif // !GAME_MANAGER_H