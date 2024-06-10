#include <SDL_image.h>
#include "PlatformManager.h"

constexpr int commonThema = 3;

enum platformThema {
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

PlatformManager::PlatformManager() {
	renderer = nullptr;
	texture = nullptr;
}
PlatformManager::PlatformManager(SDL_Renderer* renderer) {
	this->renderer = renderer;
	texture = IMG_LoadTexture(renderer, "Pixel Adventure 1/Terrain/Terrain (16x16).png");
}
void PlatformManager::loadPlatformDataFromCSVFile(std::string fileName) {
	//이후에 csv 읽기 기능을 추가할 때 :
	//tempPlatformData 배열에 정보를 담은 다음, platformData 에 옮겨담는 방식 대신
	//처음부터 platformData 에 정보를 담는 방식으로 가라.

	/*short tempPlatformData[maxPlatformXYIdx][maxPlatformXYIdx] = {
		{    3,PNON,PNON,   3,PNON,PNON,PNON,PNON,   0,   2 },
		{    9,PNON,PNON,   9,PNON,PNON,PNON,PNON,  12,   8 },
		{    9,PNON,PNON,   9,PSRC,PNON,PNON,PNON,PNON,   9 },
		{    9,PNON,PNON,   6,   1,   2,PNON,PNON,PNON,   9 },
		{    9,PNON,PNON,   6,   7,  10,   2,PNON,PNON,   9 },
		{    9,PNON,PNON,  12,  13,  13,  14,PNON,PNON,   9 },
		{    9,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,   9 },
		{    9,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,   9 },
		{    6,   1,   1,   1,   1,   1,   1,   1,   1,   8 },
		{   12,  13,  13,  13,  13,  13,  13,  13,  13,  14 },
	};*/
	short tempPlatformData[maxPlatformXYIdx][maxPlatformXYIdx] = {
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,   3,PNON,PNON,   3,PNON,PNON,PNON,PNON,   0,   2,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,   9,PNON,PNON,   9,PNON,PNON,PNON,PNON,  12,  14,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,   9,PNON,PNON,  15,PSRC,PNON,PNON,PNON,PNON,   3,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,   9,PNON,PNON,   0,   2,  21,PNON,PNON,PNON,   9,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,   9,PNON,PNON,   6,  10,   1,   2,PNON,PNON,   9,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,   9,PNON,PNON,  12,  13,  13,  14,PNON,PNON,   9,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,   9,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,   9,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,  15,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,  15,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,   0,   1,   1,   1,   1,   1,   1,   1,   1,   2,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,  12,  13,  13,  13,  13,  13,  13,  13,  13,  14,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
	};
	/*short tempPlatformData[maxPlatformXYIdx][maxPlatformXYIdx] = {
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,   0,   1,   1,   1,   2,PNON,PNON },
		{ PNON,PNON,PNON,   6,   4,  13,   5,   8,PNON,PNON },
		{ PNON,PNON,PNON,   6,   8,PSRC,   6,   8,PNON,PNON },
		{ PNON,PNON,PNON,   6,  10,   1,  11,   8,PNON,PNON },
		{ PNON,PNON,PNON,  12,  13,  13,  13,  14,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
		{ PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON,PNON },
	};*/
	for (int row = 0; row < maxPlatformXYIdx; ++row) {
		for (int col = 0; col < maxPlatformXYIdx; ++col) {
			if (tempPlatformData[row][col] >= 0)
				tempPlatformData[row][col] += commonThema * 100; // <- theme
		}
	}
	for (int i = 0; i < maxPlatformXYIdx; ++i) {
		for (int j = 0; j < maxPlatformXYIdx; ++j) {
			platformData[i][j] = tempPlatformData[i][j];
		}
	}
}
void PlatformManager::draw(int xMovement, int yMovement) {
	for (int row = 0; row < maxPlatformXYIdx; ++row) {
		for (int col = 0; col < maxPlatformXYIdx; ++col) {
			short singlePlatformData = platformData[row][col];
			if (singlePlatformData == -1)
				continue;

			short platformThema = singlePlatformData / 100;
			short platformType = singlePlatformData % 100;
			SDL_Rect srcRect = {
				// img src coord : (0 + idx / 3, 0 + idx % 3)
				16 * (
					7 * (platformThema / 3) +
					1 * (platformType  % 6)),
				16 * (
					5 * (platformThema % 3) +
					1 * (platformType  / 6)),
				16, 16
			};
			SDL_Rect dstRect = {
				PlayerTextureXYSize * col + xMovement,
				PlayerTextureXYSize * row - yMovement,
				PlayerTextureXYSize, PlayerTextureXYSize
			};
			SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
		}
	}
}

void PlatformManager::setPlayerCoordOnStartPoint(class Player* player) {
	for (int row = 0; row < maxPlatformXYIdx; ++row) {
		for (int col = 0; col < maxPlatformXYIdx; ++col) {
			if (platformData[row][col] == PSRC) {
				short xCoord = +PlayerTextureXYSize * col;
				short yCoord = -PlayerTextureXYSize * row;
				player->setCoord(xCoord, yCoord);
				break;
			}
		}
	}
}
bool PlatformManager::isPlatformExist(int xCoord, int yCoord) {
	int pointRow = -yCoord / PlayerTextureXYSize;
	int pointCol = xCoord / PlayerTextureXYSize;
	//! 나중에 여기서 문제가 생긴다면, 'xCoord < 0'일 가능성이 있다.
	//! 주시하도록.
	if (!(0 <= pointRow && pointRow < maxPlatformXYIdx)) return false;
	if (!(0 <= pointCol && pointCol < maxPlatformXYIdx)) return false;
	
	if (0 <= platformData[pointRow][pointCol])
		return true;
	else
		return false;
}
