#pragma once
#ifndef PLATFORM_MANAGER_H
#define PLATFORM_MANAGER_H

#include <SDL.h>
#include <string>
#include "Player.h"

constexpr int maxPlatformXYIdx{ 30 };
constexpr short PNON{ -1 };
constexpr short PSRC{ -2 };
constexpr short PDST{ -3 };

class PlatformManager {
public:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	short platformData[maxPlatformXYIdx][maxPlatformXYIdx] = {};

	PlatformManager();
	PlatformManager(SDL_Renderer* renderer);
	void loadPlatformDataFromCSVFile(std::string fileName);
	void draw(int xMovement, int yMovement);

	void setPlayerCoordOnStartPoint(class Player* player);
	bool isPlatformExist(int xCoord, int yCoord);

};

#endif // !PLATFORM_MANAGER_H