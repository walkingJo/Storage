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
	const Uint8* keyState;
	bool isMousePressed; //언젠가는 rMouse, lMouse로 분리한다.
	bool isMouseMoved;

	bool processRunning;
};

class GameEngine {
public:
	GameEngine() {
		pEngine = new PhysicsEngine();
		gEngine = new GraphicEngine();
		manager = nullptr;

		input = new InputProcessor();

		objects = new Object * [MaxObjects];
		objSize = 0;

		clock = 0;
		deltaTime = 0.1f;
		isRunning = true;
	}
	~GameEngine() {
		delete pEngine;
		delete gEngine;

		delete manager;
		delete input;

		delete objects;
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
			//object와 manager의 update()에는 충돌 판정이 필요하므로,
			//    pEngine->object->Manager 의 순서로 update()를 실행하는 것이다.
			timeUpdate();
			manager->setObjects();
			pEngine->update(objects, objSize);
			manager->update(input);
			gEngine->update(objects, objSize);
#pragma endregion
		}
	}

	void resetObjects() {
		for (int i = 0; i < objSize; ++i)
			objects[i] = nullptr;
	}
	void addObject(Object* obj) {
		//
	}

private:
#pragma region Engine Fields


	PhysicsEngine* pEngine;
	GraphicEngine* gEngine;
	GameManager* manager;

	InputProcessor* input;

	Object** objects;
	short objSize;
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