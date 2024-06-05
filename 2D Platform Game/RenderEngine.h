#pragma once
#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <SDL.h>
	
class RenderEngine {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	const char* projectTitle = "project title";
	static const int ScreenWidth = 960;
	static const int ScreenHeight = 640;

	RenderEngine();
	void init();
	void renderClear();
	void renderPresent();
	void release();
	
	SDL_Renderer* getRenderer();

};

#endif // !RENDER_ENGINE_H