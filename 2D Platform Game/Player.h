#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <ctime>
#include "PlatformManager.h"

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
	SDL_Renderer* renderer;
	class PlatformManager* platform;

	CharacterType playerType;
	SDL_Texture* texture;

	int xCoord, yCoord;
	int xSpeed, ySpeed;

	PlayerStateType state;
	FlipStateType flipState;
	clock_t stateStartTime;
	
	void setTextureWithState();
	void setState(PlayerStateType state);
	int getAnimationIdxWithTime(int length);
	
	bool isPlayerTouchedGround();
	bool isPlayerTouchedOnWallRightSide();
	bool isPlayerTouchedOnWallLeftSide();
	bool isPlayerTouchedOnCeiling();
	//bool isHeightEnoughToHangOnWall() {
	//	if (yCoord - TextureXYSize >= TextureXYSize / 2 - RenderEngine::ScreenHeight)
	//		return true;
	//	return false;
	//}

	void setRightMoveSpeed();
	void setLeftMoveSpeed();
	void setJumpSpeed();
	void setRightWallJumpSpeed();
	void setLeftWallJumpSpeed();

	//void doNormalHorizontalMoveProcess(InputProcessor* input, PlayerStateType movingState, PlayerStateType nonMovingState) {
	//	const bool& isRightKeyPressed = input->isRightKeyPressed();
	//	const bool& isLeftKeyPressed = input->isLeftKeyPressed();
	//	if (isRightKeyPressed ^ isLeftKeyPressed) {
	//		setState(movingState);
	//		if (isRightKeyPressed)	setRightMoveSpeed();
	//		else					setLeftMoveSpeed();
	//	}
	//	else {
	//		setState(nonMovingState);
	//	}
	//}
	//void doNormalJumpProcess(InputProcessor* input, PlayerStateType jumpState) {
	//	static bool isSpaceKeyTypePressed;
	//	isSpaceKeyTypePressed = input->getKeyState(SDL_SCANCODE_SPACE) == KeyboardState::PRESSED;
	//	if (isSpaceKeyTypePressed) {
	//		setState(jumpState);
	//		setJumpSpeed();
	//	}
	//	//break; // <- 이 구문을 꼭 넣어야 하는데..., switch 문 안에 넣지 않으면 의미가 없다 ㅋ
	//}

public:
	Player();
	Player(CharacterType playerType, SDL_Renderer* renderer, class PlatformManager* platform);
	~Player();

	void setCoord(int xCoord, int yCoord);
	void setSpeed(int xSpeed, int ySpeed);
	void update(class InputProcessor* input);
	void draw();

/**/PlayerStateType getState() {
/**/	return state;
/**/}

};

#endif // !PLAYER_H