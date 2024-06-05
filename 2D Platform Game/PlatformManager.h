#pragma once
#ifndef PLATFORM_MANAGER_H
#define PLATFORM_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Player.h"

constexpr int maxPlatformXYIdx = 10;

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

	PlatformManager() {
		renderer = nullptr;
		texture = nullptr;
	}
	PlatformManager(SDL_Renderer* renderer) {
		this->renderer = renderer;
		texture = IMG_LoadTexture(renderer, "Pixel Adventure 1/Terrain/Terrain (16x16).png");
	}
	void loadPlatformDataFromCsvFile(std::string fileName) {
		//platformData...
		short tempPlatformData[maxPlatformXYIdx][maxPlatformXYIdx] = {
			{ 300, 302,  -1,  -1,  -1,  -1,  -1,  -1, 300, 302 },
			{ 306, 308,  -1,  -1,  -1,  -1,  -1,  -1, 306, 308 },
			{ 306, 308,  -1,  -1,  -1,  -1,  -1,  -1, 306, 308 },
			{ 306, 308,  -1,  -1,  -1,  -1,  -1,  -1, 306, 308 },
			{ 306, 308,  -1,  -1,  -1,  -1,  -1,  -1, 306, 308 },
			{ 306, 308,  -1,  -1,  -1,  -1,  -1,  -1, 306, 308 },
			{ 306, 308,  -1,  -1,  -1,  -1,  -1,  -1, 306, 308 },
			{ 306, 310, 301, 301, 301, 301, 301, 301, 311, 308 },
			{ 306, 307, 307, 307, 307, 307, 307, 307, 307, 308 },
			{ 312, 313, 313, 313, 313, 313, 313, 313, 313, 314 },
		};
		for (int i = 0; i < maxPlatformXYIdx; ++i) {
			for (int j = 0; j < maxPlatformXYIdx; ++j) {
				platformData[i][j] = tempPlatformData[i][j];
			}
		}
	}
	void draw() {
		for (int row = 0; row < maxPlatformXYIdx; ++row) {
			for (int col = 0; col < maxPlatformXYIdx; ++col) {
				short singlePlatformData = platformData[row][col];
				if (singlePlatformData == -1)
					continue;

				short platformThema = singlePlatformData / 100;
				short platformType = singlePlatformData % 100;
				SDL_Rect srcRect = {
					// img src coord : (0 + idx / 3, 0 + idx % 3)
					16 * (7 * platformThema / 3 + platformType % 6),
					16 * (5 * platformThema % 3 + platformType / 6),
					16, 16
				};
				SDL_Rect dstRect = {
					TextureXYSize * col, TextureXYSize * row,
					TextureXYSize, TextureXYSize
				};
				SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
			}
		}
	}

	//void setPlayerStartCoord(Player* player, int xCoord, int yCoord) {
	//	player->setCoord(xCoord, yCoord);
	//}

};

#endif // !PLATFORM_MANAGER_H