#pragma once
#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "GameEngine.h"

class PhysicsEngine {
public:
	PhysicsEngine() {
		engine = nullptr;
	}

	void init(GameEngine* engine) {
		this->engine = engine;
	}
	void update() {
		//
	}

private:
	class GameEngine* engine;
};

#endif // !PHYSICS_ENGINE_H