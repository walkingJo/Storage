#pragma once
#ifndef PLATFORM_MANAGER_H
#define PLATFORM_MANAGER_H

#include <SDL.h>
#include <string>
#include "Player.h"

constexpr int maxPlatformXYIdx{ 10 };
constexpr short PSRC{ -2 };
constexpr short PDST{ -3 };

enum class PlatformType {
	OUTLINE_ROCK,
	OUTLINE_WOOD,
	OUTLINE_LEAF,

	GROUND_GREEN,
	GROUND_ORANGE,
	GROUND_PINK,

	BLOCK_COPPER,
	BLOCK_SILBER,
	BLOCK_PLASTIC,
	BLOCK_BRICK,
	BLOCK_GOLD,
};

class PlatformManager {
public:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	short platformData[maxPlatformXYIdx][maxPlatformXYIdx] = {};

	PlatformManager();
	PlatformManager(SDL_Renderer* renderer);
	void loadPlatformDataFromCsvFile(std::string fileName);
	void draw();

	void setPlayerOnStartCoord(class Player* player);

};

#endif // !PLATFORM_MANAGER_H