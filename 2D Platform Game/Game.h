#pragma once
#ifndef GAME_H
#define GAME_H

#include <ctime>
#include "RenderEngine.h"
#include "PlatformManager.h"
#include "InputProcessor.h"
#include "Player.h"

class Game {
private:
	RenderEngine renderer;
	PlatformManager platform;
	InputProcessor input;
	bool isRunning;

	Player playerObj;

	clock_t time;
	clock_t deltaTime;

	void init() {
		renderer = RenderEngine();
		renderer.init();

		platform = PlatformManager(renderer.getRenderer());
		platform.loadPlatformDataFromCsvFile("null");

		input = InputProcessor();
		isRunning = true;

		playerObj = Player(CharacterType::NINJA_FROG, renderer.getRenderer());
		//platform.

		time = clock();
		deltaTime = 1000 / 60;
	}
	void getInput() {
		input.update();
		isRunning = input.isRunning();
	}
	void update() {
		playerObj.update(&input);
	}
	void render() {
		renderer.renderClear();

		platform.draw();
		playerObj.draw();

		renderer.renderPresent();
	}
	void wait() {
		clock_t oldClock = clock();
		while (1000.0f / 60 < clock() - oldClock);
	}
	void release() {
		renderer.release();
		SDL_Quit();
	}

public:
	Game() {
		init();
	}
	void run() {
		while (isRunning) {
			getInput();
			update();
			render();
			wait();
		}
		release();
	}
};

#endif // !GAME_H