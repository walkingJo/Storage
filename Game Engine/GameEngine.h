#pragma once
#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameManager.h"
#include "PhysicsEngine.h"
#include "GraphicEngine.h"

constexpr int MaxObjects = 50;

class MouseCoord {
public:
	int xCoord;
	int yCoord;

	MouseCoord(): xCoord(0), yCoord(0) {}
	MouseCoord(int x, int y): xCoord(x), yCoord(y) {}
};

class InputProcessor {
public:
	InputProcessor() {
		newCoord = { 0,0 };
		oldCoord = { 0,0 };
		isMousePressed = false;
		isMouseMoved = false;
		keyState = SDL_GetKeyboardState(NULL);

		processRunning = true;
	}
	void update() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				processRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				newCoord = MouseCoord(event.button.x, event.button.y);
				oldCoord = MouseCoord(event.button.x, event.button.y);
				isMousePressed = true;
				isMouseMoved = false;
				break;
			case SDL_MOUSEBUTTONUP:
				isMousePressed = false;
				isMouseMoved = false;
				break;
			case SDL_MOUSEMOTION:
				if (isMousePressed) {
					newCoord = MouseCoord(event.motion.x, event.motion.y);
				}
				isMouseMoved = true;
				break;
			}
		}
		keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_ESCAPE]) {
			processRunning = false;
		}
	}

	const MouseCoord& getMouseCoord() {
		return newCoord;
	}
	const MouseCoord& getMouseDeltaCoord() {
		return MouseCoord(
			newCoord.xCoord - oldCoord.xCoord,
			newCoord.yCoord - oldCoord.yCoord
		);
	}
	bool isKeyPressed(int keyCode) {
		return keyState[keyCode];
	}
	bool isRunning() {
		return processRunning;
	}

private:
	MouseCoord newCoord;
	MouseCoord oldCoord;
	bool isMousePressed;
	bool isMouseMoved;
	const Uint8* keyState;

	bool processRunning;
};

class GameEngine {
public:
	GameEngine() {
		pEngine = new PhysicsEngine();
		gEngine = new GraphicEngine();
		manager = nullptr;

		input = new InputProcessor();

		livingObjects = new Object * [MaxObjects];	livingObjectsCount = 0;
		deadObjects = new Object * [MaxObjects];	deadObjectsCount = 0;

		clock = 0;
		deltaTime = 0.1f;
		isRunning = true;
	}
	~GameEngine() {
		delete pEngine;
		delete gEngine;

		//delete input;
	
		for (int i = 0; i < livingObjectsCount; ++i)
			delete livingObjects[i];
		for (int i = 0; i < deadObjectsCount; ++i)
			delete deadObjects[i];
		delete livingObjects;
		delete deadObjects;
	}

	void init() {
		gEngine->init();
		pEngine->init();
	}
	void setManager(GameManager* manager) {
		this->manager = manager;
		this->manager->setEngines(this, gEngine);
	}
	void run() {
		while (isRunning) {
#pragma region Get Input
			input->update();
			isRunning = input->isRunning();
#pragma endregion
#pragma region Update
			timeUpdate();
			pEngine->update(this);
			for (int i = 0; i < livingObjectsCount; ++i)
				livingObjects[i]->update();
			manager->update(input); //manager의 연산에는 충돌 판정이 필요하므로 `pEngine`->`Manager`의 순서로 `update()`를 진행하는 것임
			//gEngine->update(this);
			gEngine->update(livingObjects, livingObjectsCount);
#pragma endregion
		}
	}

private:
#pragma region Engine Fields
	friend GraphicEngine;
	friend PhysicsEngine;

	PhysicsEngine* pEngine;
	GraphicEngine* gEngine;
	GameManager* manager;

	InputProcessor* input;

	Object** livingObjects;	int livingObjectsCount;
	Object** deadObjects;	int deadObjectsCount;
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