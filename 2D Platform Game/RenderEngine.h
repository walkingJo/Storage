#pragma once
#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <SDL.h>
#include "PlatformManager.h"
	
class RenderEngine {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	int cameraXCoord;
	int cameraYCoord;

	void setCameraCoordWithDstCoord(int xCoord, int yCoord);

public:
	const char* projectTitle = "project title";
	static const int ScreenWidth = 960;
	static const int ScreenHeight = 640;

	static const int centorXCoord = RenderEngine::ScreenWidth / 2;
	static const int centorYCoord = -RenderEngine::ScreenHeight / 2;

	RenderEngine();
	void init();
	void release();

	void renderClear();
	void setCameraCoord(int xCoord = centorXCoord, int yCoord = centorYCoord);
	void setCameraCoordWithPlayerCoord(Player* playerObj);
	void setCameraCoordWithPlayerLookUp(Player* playerObj);
	void setCameraCoordWithPlayerLookDown(Player* playerObj);
	void draw(PlatformManager* platform);
	void draw(Player* playerObj);
	void renderPresent();
	
	SDL_Renderer* getRenderer();

};

#endif // !RENDER_ENGINE_H