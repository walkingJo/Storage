#pragma once
#ifndef GRAPHIC_ENGINE_H
#define GRAPHIC_ENGINE_H

#include <SDL.h>

class GraphicEngine {
public:
	GraphicEngine() {
		window = nullptr;
		renderer = nullptr;
		depthBuffer = nullptr;
	}

	void init() {
		window = SDL_CreateWindow(projectTitle, 100+0, 100-1, ScreenWidth, ScreenHeight, 0);
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
	//void update(GameEngine* engine) {
	//	clear(0x4E, 0xC9, 0xB0);
	//	for (int i = 0; i < engine->livingObjectsCount; ++i)
	//		drawObject(engine->livingObjects[i]);
	//	renderPresent();
	//}
	void update(Object** objects, int objectSize) {
		clear(0x4E, 0xC9, 0xB0);
		for (int i = 0; i < objectSize; ++i)
			drawObject(objects[i]);
		renderPresent();
	}

private:
	const char* projectTitle = "3D Game Engine Project";

#pragma region Rendering Fields
	int ScreenWidth{/*1920*/800 };
	int ScreenHeight{/*1080*/600 };
	float ScreenNear{ 100.0f };
	float ScreenFar{ 10000.0f };

	SDL_Window* window;
	SDL_Renderer* renderer;
	float** depthBuffer;
#pragma endregion

	void clear(int r, int g, int b) {
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderClear(renderer);
		for (int row = 0; row < ScreenHeight; ++row) {
			for (int col = 0; col < ScreenWidth; ++col) {
				depthBuffer[row][col] = ScreenFar;
			}
		}
	}
	void drawModel(Model* model) {
		return;
	}
	void drawObject(Object* object) {
		return;
	}
	void renderPresent() {
		SDL_RenderPresent(renderer);
	}
};

#endif // !GRAPHIC_ENGINE_H