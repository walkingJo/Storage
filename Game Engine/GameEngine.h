#pragma once
#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameManager.h"
#include "GraphicEngine.h"
#include "PhysicsEngine.h"
#include "Object.h"

constexpr int MaxObjects = 50;

class GameEngine {
public:
	GameEngine() {
		gEngine = new GraphicEngine();
		pEngine = new PhysicsEngine();
		manager = nullptr;

		livingObjects = new Object * [MaxObjects];	livingObjectsCount = 0;
		deadObjects = new Object * [MaxObjects];	deadObjectsCount = 0;

		clock = 0;
		deltaTime = 0.1f;
		isRunning = true;
	}
	~GameEngine() {
		delete gEngine;
		delete pEngine;
	
		for (int i = 0; i < livingObjectsCount; ++i)
			delete livingObjects[i];
		for (int i = 0; i < deadObjectsCount; ++i)
			delete deadObjects[i];
		delete livingObjects;
		delete deadObjects;
	}

	void init() {
		gEngine->init(this);
		pEngine->init(this);
	}
	void setManager(class GameManager* manager) {
		this->manager = manager;
		this->manager->setEngine(this);
	}
	void run() {
		while (isRunning) {
#pragma region Get Input
			// :D
#pragma endregion

#pragma region Update
			timeUpdate();
			for (int i = 0; i < livingObjectsCount; ++i)
				livingObjects[i]/*->action*/;
			gEngine->update();
			pEngine->update();
#pragma endregion
		}
	}
private:
#pragma region Engine Fields
	class GraphicEngine* gEngine;
	class PhysicsEngine* pEngine;
	class GameManager* manager;

	//friend GraphicEngine;
	//friend PhysicsEngine;
	Object** livingObjects;	int livingObjectsCount;
	Object** deadObjects;	int deadObjectsCount;

	//key input
#pragma endregion

#pragma region Time Fields
	Uint32 clock;
	float deltaTime;
	bool isRunning;

	void timeUpdate() {
		while (deltaTime < 0.01f) {
			deltaTime = (SDL_GetTicks() - clock) / 1000.0f;
		}
		clock = SDL_GetTicks();
	}
#pragma endregion
};

#endif // !GAME_ENGINE_H