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
	//�ϴ� GraphicEngine�� �� ��������� �������� ��ġ�ص���
};

#endif // !PHYSICS_ENGINE_H