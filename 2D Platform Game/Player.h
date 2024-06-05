#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <ctime>

constexpr int TextureXYSize = 64;

enum class CharacterType {
	MASK_DUDE,
	VIRTUAL_GUY,
	PINK_MAN,
	NINJA_FROG,
};

enum class PlayerStateType {
	NON,
	IDLE,
	RUN,
	JUMP,
	DOUBLE_JUMP,
	WALL_JUMP,
	FALL_JUMP,
	FALL_DOUBLE_JUMP,
	HIT,
};

enum class FlipStateType {
	LOOK_AT_RIGHT,
	LOOK_AT_LEFT,
	LOOK_AT_RIGHT_ON_WALL,
	LOOK_AT_LEFT_ON_WALL,
};

class Player {
private:
	SDL_Renderer* renderer; // ← 이 부분이 다음 기능 구현에서 핵심적이다.

	CharacterType playerType;
	SDL_Texture* texture;

	int xCoord, yCoord;
	int xSpeed, ySpeed;

	PlayerStateType state;
	FlipStateType flipState;
	clock_t stateStartTime;
	
	void setTextureWithState();
	void setState(PlayerStateType state);
	int getIdxWithTime(int length);
	
	bool isPlayerTouchedGround();
	bool isPlayerTouchedOnWallRightSide();
	bool isPlayerTouchedOnWallLeftSide();
	bool isPlayerTouchedOnCeiling();
	bool isHeightEnoughToHangOnWall();

	void setRightMoveSpeed();
	void setLeftMoveSpeed();
	void setJumpSpeed();
	void setRightWallJumpSpeed();
	void setLeftWallJumpSpeed();

public:
	Player();
	Player(CharacterType playerType, SDL_Renderer* renderer);
	~Player();

	void setCoord(int xCoord, int yCoord);
	void setSpeed(int xSpeed, int ySpeed);
	void update(class InputProcessor* input);
	void draw();

};

#endif // !PLAYER_H