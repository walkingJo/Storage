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
	//게임 엔진을 조작해 게임을 진행하는 코드를 업데이트에 넣도록.
	//어쩌면 GameManager를 다수 생성해서 특정 상황에서 교체해 사용하는 것도 가능할듯
	//    >> 마치 Scene과 같은 활용
	virtual void update() {
		return;
	}
private:
	class GameEngine* engine;
};

#endif // !GAME_MANAGER_H