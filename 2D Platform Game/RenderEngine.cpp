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
void RenderEngine::drawBackGround(BackgroundType background) {
	std::string filePath = "Pixel Adventure 1/Background/";
	switch (background) {
	case BackgroundType::BLUE:		filePath += "Blue";		break;
	case BackgroundType::BROWN:		filePath += "Brown";	break;
	case BackgroundType::GRAY:		filePath += "Gray";		break;
	case BackgroundType::GREEN:		filePath += "Green";	break;
	case BackgroundType::PINK:		filePath += "Pink";		break;
	case BackgroundType::PURPLE:	filePath += "Purple";	break;
	case BackgroundType::YELLOW:	filePath += "Yellow";	break;
	}
	filePath += ".png";

	const int backgroundTextureXYSize = 64 * 1;
	SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
	SDL_Rect srcRect = {
		0, 0,
		backgroundTextureXYSize, backgroundTextureXYSize
	};
	for (int row = 0; row <= ScreenWidth; row += backgroundTextureXYSize) {
		for (int col = 0; col <= ScreenHeight; col += backgroundTextureXYSize) {
			SDL_Rect dstRect = {
				row, col,
				backgroundTextureXYSize, backgroundTextureXYSize
			};
			SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
		}
	}

	SDL_DestroyTexture(texture);
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
