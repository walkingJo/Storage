#pragma once
#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "Object.h"

class PhysicsEngine {
public:
	PhysicsEngine() {
		//
	}

	void init() {
		return;
	}
	void update(GameManager* manager) {
		Object** objects = manager->objects;
		int objSize = manager->objSize;
		return;
	}

private:
	//일단 GraphicEngine이 다 만들어지기 전까지는 방치해두자
};

#endif // !PHYSICS_ENGINE_H