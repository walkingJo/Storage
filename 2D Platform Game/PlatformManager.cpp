#include <SDL_image.h>
#include "PlatformManager.h"

PlatformManager::PlatformManager() {
	renderer = nullptr;
	texture = nullptr;
}
PlatformManager::PlatformManager(SDL_Renderer* renderer) {
	this->renderer = renderer;
	texture = IMG_LoadTexture(renderer, "Pixel Adventure 1/Terrain/Terrain (16x16).png");
}
void PlatformManager::loadPlatformDataFromCsvFile(std::string fileName) {
	//이후에 csv 읽기 기능을 추가할 때 :
	//tempPlatformData 배열에 정보를 담은 다음, platformData 에 옮겨담는 방식 대신
	//처음부터 platformData 에 정보를 담는 방식으로 가라.

	short tempPlatformData[maxPlatformXYIdx][maxPlatformXYIdx] = {
		{ 300, 302,  -1,  -1,  -1,  -1,  -1,  -1, 300, 302 },
		{ 306, 308,  -1,  -1,  -1,  -1,  -1,  -1, 306, 308 },
		{ 306, 308,  -1,  -1,PSRC,  -1,  -1,  -1, 306, 308 },
		{ 306, 308,  -1, 300, 301, 302,  -1,  -1, 306, 308 },
		{ 306, 308,  -1, 306, 307, 310, 302,  -1, 306, 308 },
		{ 306, 308,  -1, 312, 313, 313, 314,  -1, 306, 308 },
		{ 306, 308,  -1,  -1,  -1,  -1,  -1,  -1, 306, 308 },
		{ 306, 308,  -1,  -1,  -1,  -1,  -1,  -1, 306, 308 },
		{ 306, 310, 301, 301, 301, 301, 301, 301, 311, 308 },
		{ 312, 313, 313, 313, 313, 313, 313, 313, 313, 314 },
	};
	for (int i = 0; i < maxPlatformXYIdx; ++i) {
		for (int j = 0; j < maxPlatformXYIdx; ++j) {
			platformData[i][j] = tempPlatformData[i][j];
		}
	}
}
void PlatformManager::draw() {
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

void PlatformManager::setPlayerOnStartCoord(class Player* player) {
	for (int row = 0; row < maxPlatformXYIdx; ++row) {
		for (int col = 0; col < maxPlatformXYIdx; ++col) {
			if (platformData[row][col] == PSRC) {
				short xCoord = +TextureXYSize * col;
				short yCoord = -TextureXYSize * row;
				player->setCoord(xCoord, yCoord);
				break;
			}
		}
	}
}
