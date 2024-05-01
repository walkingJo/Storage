#pragma once
#ifndef GRAPHIC_ENGINE_H
#define GRAPHIC_ENGINE_H

#include "GameEngine.h"

class GraphicEngine {
public:
	GraphicEngine() {
		window = nullptr;
		renderer = nullptr;
		engine = nullptr;
		depthBuffer = nullptr;
	}

	void init(class GameEngine* engine) {
		this->engine = engine;

		window = SDL_CreateWindow("Project Name", 0, -1, ScreenWidth, ScreenHeight, 0);
		if (!window) {
			SDL_Log("Failed to create window: %s", SDL_GetError());
			throw;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!renderer) {
			SDL_Log("Failed to create renderer: %s", SDL_GetError());
			throw;
		}

		depthBuffer = new float* [ScreenHeight];
		for (int row = 0; row < ScreenHeight; ++row) {
			depthBuffer[row] = new float[ScreenWidth];
			for (int col = 0; col < ScreenWidth; ++col)
				depthBuffer[row][col] = ScreenFar;
		}
	}
	void update() {
		return;
	}
private:
	class GameEngine* engine;
	//engine->livingObjects

#pragma region Rendering Fields
	int ScreenWidth{/*1920*/800 };
	int ScreenHeight{/*1080*/600 };
	float ScreenNear{ 100.0f };
	float ScreenFar{ 10000.0f };

	SDL_Window* window;
	SDL_Renderer* renderer;
	float** depthBuffer;
#pragma endregion
};

#endif // !GRAPHIC_ENGINE_H