#include <SDL_image.h>
#include <string>
#include "RenderEngine.h"
#include "Player.h"

RenderEngine::RenderEngine() {
	window = nullptr;
	renderer = nullptr;
}
void RenderEngine::init() {
	constexpr int startXCoord = 100;
	constexpr int startYCoord = 100;
	window = SDL_CreateWindow(
		projectTitle,
		startXCoord + 0, startYCoord - 1,
		ScreenWidth, ScreenHeight, 0);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		throw;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		throw;
	}
}
void RenderEngine::renderClear() {
	SDL_SetRenderDrawColor(renderer, 0x13, 0x6E, 0x2D, 0xFF);
	SDL_RenderClear(renderer);
}
void RenderEngine::renderPresent() {
	SDL_RenderPresent(renderer);
}
void RenderEngine::release() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

SDL_Renderer* RenderEngine::getRenderer() {
	return renderer;
}
