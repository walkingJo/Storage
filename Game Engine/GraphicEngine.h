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
		constexpr int startXCoord = 0;
		constexpr int startYCoord = 0;
		window = SDL_CreateWindow(
			projectTitle,
			startXCoord + 0, startYCoord - 1,
			ScreenWidth,ScreenHeight, 0);
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
	void update(GameManager* manager) {
		clear(0x4E, 0xC9, 0xB0);
		for (int i = 0; i < manager->objSize; ++i)
			drawObject(manager->objects[i]);
		renderPresent();
	}

private:
	const char* projectTitle = "3D Game Engine Project";

#pragma region Rendering Fields
	int ScreenWidth{ 1706 };
	int ScreenHeight{ 1066 };
	/*int ScreenWidth{ 1920 };
	int ScreenHeight{ 1080 };*/
	/*int ScreenWidth{ 800 };
	int ScreenHeight{ 600 };*/
	float ScreenNear{ 100.0f };
	float ScreenFar{ 10000.0f };

	SDL_Window* window;
	SDL_Renderer* renderer;
	float** depthBuffer;
#pragma endregion

	void clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderClear(renderer);
		for (int row = 0; row < ScreenHeight; ++row) {
			for (int col = 0; col < ScreenWidth; ++col) {
				depthBuffer[row][col] = ScreenFar;
			}
		}
	}
	//얘는 아직 안쓴다
	void drawLine() {
		return;
	}
	//카메라에 대한 상대좌표만을 사용
	void drawTriangle(Vector3 vec1, Vector3 vec2, Vector3 vec3) {
		//깊이 버퍼의 내용은 물론 포함되어야 함
		return;
	}
	void drawModel(Model* model) {
		Model newModel = Model(*model);
		//newModel의 기저변환 코드.

		for (int i = 0; i < newModel.idxSize; ++i) {
			drawTriangle(
				newModel.vertices[3 * i + 0],
				newModel.vertices[3 * i + 1],
				newModel.vertices[3 * i + 2]);
		}
	}
	void drawObject(Object* object) {
		drawModel(object->model);
	}
	void renderPresent() {
		SDL_RenderPresent(renderer);
	}
};

#endif // !GRAPHIC_ENGINE_H