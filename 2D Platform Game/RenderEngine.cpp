#include <SDL_image.h>
#include <string>
#include "RenderEngine.h"
#include "Player.h"

RenderEngine::RenderEngine() {
	window = nullptr;
	renderer = nullptr;

	cameraXCoord = 0;
	cameraYCoord = 0;
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
void RenderEngine::release() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void RenderEngine::renderClear() {
	SDL_SetRenderDrawColor(renderer, 0x13, 0x6E, 0x2D, 0xFF);
	SDL_RenderClear(renderer);
}
void RenderEngine::renderPresent() {
	SDL_RenderPresent(renderer);
}
void RenderEngine::setCameraCoord(int xCoord, int yCoord) {
	this->cameraXCoord = xCoord;
	this->cameraYCoord = yCoord;
}
void RenderEngine::setCameraCoordWithDstCoord(int xCoord, int yCoord) {
	int dstXCoord = (int)(cameraXCoord + (xCoord - cameraXCoord) * 0.1);
	int dstYCoord = (int)(cameraYCoord + (yCoord - cameraYCoord) * 0.1);

	setCameraCoord(dstXCoord, dstYCoord);
}
void RenderEngine::setCameraCoordWithPlayerCoord(Player* playerObj) {
	int playerXCoord = playerObj->getXCoord() + PlayerTextureXYSize / 2;
	int playerYCoord = playerObj->getYCoord() - PlayerTextureXYSize / 2;
	setCameraCoordWithDstCoord(playerXCoord, playerYCoord);
}
void RenderEngine::setCameraCoordWithPlayerLookUp(Player* playerObj) {
	int dstXCoord = playerObj->getXCoord() + PlayerTextureXYSize / 2;
	int dstYCoord = playerObj->getYCoord() - PlayerTextureXYSize / 2 + 500;
	setCameraCoordWithDstCoord(dstXCoord, dstYCoord);
}
void RenderEngine::setCameraCoordWithPlayerLookDown(Player* playerObj) {
	int dstXCoord = playerObj->getXCoord() + PlayerTextureXYSize / 2;
	int dstYCoord = playerObj->getYCoord() - PlayerTextureXYSize / 2 - 500;
	setCameraCoordWithDstCoord(dstXCoord, dstYCoord);
}
void RenderEngine::draw(PlatformManager* platform) {
	platform->draw(
		centorXCoord - cameraXCoord,
		centorYCoord - cameraYCoord
	);
}
void RenderEngine::draw(Player* playerObj) {
	playerObj->drawWithCameraCoord(cameraXCoord, cameraYCoord);
}

SDL_Renderer* RenderEngine::getRenderer() {
	return renderer;
}
