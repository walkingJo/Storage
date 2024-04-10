#pragma once
#ifndef RENDER_ENGINE
#define RENDER_ENGINE

#include <SDL.h>
#include "Game.h"
#include "Object.h"
#include "Math.h"

constexpr int ScreenWidth{/*1720*/800 };
constexpr int ScreenHeight{/*1080*/600 };
constexpr float ScreenNear{ 100.0f };
constexpr float ScreenFar{ 10000.0f };

class RenderEngine {
public:
	RenderEngine(class Game* game);
	RenderEngine();
	virtual ~RenderEngine();

	void createWindow();

	void clear(int r, int g, int b);
	void drawObject(Object object);
	void addObject(Object object);
	void drawAllObject();

	void drawModel(Model model);
	void renderPresent();

	void moveTo(Vector3 location);
	void lookAt(Vector3 target);

	SDL_Window* window;
	SDL_Renderer* renderer;
	//float** depthBuffer;
	float depthBuffer[ScreenHeight][ScreenWidth] = {};
	class Game* game;

public:
	Object camera;
	std::vector<Object> objects;
private:
	bool isV3RenderOk(Vector3 v);
	Vector3 basisConversionWithCamera(Vector3 v);
	SDL_FPoint vector3ToFPoint(Vector3 v);
	void drawPointOnCamera(Vector3 v1, SDL_Color color);
	void drawLineOnCamera(Vector3 v1, Vector3 v2, SDL_Color color);
	void drawTriangleOnCamera(Vector3 v1, Vector3 v2, Vector3 v3, SDL_Color color);
};

#endif // !RENDER_ENGINE