#include <SDL_image.h>
#include <string>
#include <cassert>
#include <ctime>
#include "RenderEngine.h"
#include "InputProcessor.h"
#include "Player.h"

constexpr int MotionPerSecond = 1000 / 15;

void Player::setTextureWithState() {
	//		"Pixel Adventure 1/Main Characters/Player Type/File Name.png";
	//		                                   ^^^^^^^^^^^ ^^^^^^^^^
	if (texture != NULL)
		SDL_DestroyTexture(texture);
	if (renderer == NULL)
		return;

	const char characterNames[4][12] = {
		"Mask Dude",
		"Virtual Guy",
		"Pink Man",
		"Ninja Frog",
	};
	const char fileNames[7][24] = {
		"Idle (32x32).png",
		"Run (32x32).png",
		"Jump (32x32).png",
		"Double Jump (32x32).png",
		"Wall Jump (32x32).png",
		"Fall (32x32).png",
		"Hit (32x32).png",
	};

	std::string filePath = "Pixel Adventure 1/Main Characters/";
	switch (playerType) {
	case CharacterType::MASK_DUDE:		filePath += characterNames[0]; break;
	case CharacterType::VIRTUAL_GUY:	filePath += characterNames[1]; break;
	case CharacterType::PINK_MAN:		filePath += characterNames[2]; break;
	case CharacterType::NINJA_FROG:		filePath += characterNames[3]; break;
	default: throw; break;
	}
	filePath += "/";
	switch (state) {
	case PlayerStateType::IDLE:					filePath += fileNames[0];	break;
	case PlayerStateType::RUN:					filePath += fileNames[1];	break;
	case PlayerStateType::JUMP:					filePath += fileNames[2];	break;
	case PlayerStateType::DOUBLE_JUMP:			filePath += fileNames[3];	break;
	case PlayerStateType::WALL_JUMP:			filePath += fileNames[4];	break;
	case PlayerStateType::FALL_JUMP:
	case PlayerStateType::FALL_DOUBLE_JUMP:		filePath += fileNames[5];	break;
	case PlayerStateType::HIT:					filePath += fileNames[6];	break;
	default:
		break;
	}
	texture = IMG_LoadTexture(renderer, filePath.c_str());
}
void Player::setState(PlayerStateType state) {
	if (this->state == state)
		return;

	this->state = state;
	stateStartTime = clock();
	setTextureWithState();
}
int Player::getAnimationIdxWithTime(int length) {
	return ((clock() - stateStartTime) / MotionPerSecond) % length;
}

bool Player::isPlayerTouchedGround() {
	if (yCoord - TextureXYSize <= -RenderEngine::ScreenHeight)
		return true;
	return false;
}
bool Player::isPlayerTouchedOnWallRightSide() {
	if (xCoord <= 0)
		return true;
	return false;
}
bool Player::isPlayerTouchedOnWallLeftSide() {
	if (xCoord + TextureXYSize >= RenderEngine::ScreenWidth)
		return true;
	return false;
}
bool Player::isPlayerTouchedOnCeiling() {
	if (yCoord >= 0)
		return true;
	return false;
}
bool Player::isHeightEnoughToHangOnWall() {
	if (yCoord - TextureXYSize >= TextureXYSize / 2 - RenderEngine::ScreenHeight)
		return true;
	return false;
}

void Player::setRightMoveSpeed() {
	const int moveCoord = 10;
	xCoord += moveCoord;
	flipState = FlipStateType::LOOK_AT_RIGHT;
}
void Player::setLeftMoveSpeed() {
	const int moveCoord = 10;
	xCoord -= moveCoord;
	flipState = FlipStateType::LOOK_AT_LEFT;
}
void Player::setJumpSpeed() {
	const int JumpSpeed = 15;
	ySpeed = JumpSpeed;
}
void Player::setRightWallJumpSpeed() {
	const int JumpSpeed = 15;
	xSpeed = JumpSpeed;
	ySpeed = JumpSpeed;
}
void Player::setLeftWallJumpSpeed() {
	const int JumpSpeed = 15;
	xSpeed = -JumpSpeed;
	ySpeed = JumpSpeed;
}

Player::Player() {
	renderer = nullptr;
	platform = nullptr;

	this->playerType = CharacterType::MASK_DUDE;
	texture = nullptr;

	xCoord = yCoord = 0;
	xSpeed = ySpeed = 0;

	state = PlayerStateType::IDLE;
	flipState = FlipStateType::LOOK_AT_RIGHT;
	stateStartTime = 0;
}
Player::Player(CharacterType playerType, SDL_Renderer* renderer, PlatformManager* platform) {
	this->renderer = renderer;

	this->platform = platform;

	this->playerType = playerType;
	texture = nullptr;

	xCoord = yCoord = 0;
	xSpeed = ySpeed = 0;

	state = PlayerStateType::IDLE;
	flipState = FlipStateType::LOOK_AT_RIGHT;
	stateStartTime = clock();

	setState(PlayerStateType::IDLE);
}
Player::~Player() {
	SDL_DestroyTexture(texture);
}

void Player::setCoord(int xCoord, int yCoord) {
	setSpeed(0, 0);
	this->xCoord = xCoord;
	this->yCoord = yCoord;
}
void Player::setSpeed(int xSpeed, int ySpeed) {
	this->xSpeed = xSpeed;
	this->ySpeed = ySpeed;
}
void Player::update(InputProcessor* input) {
	bool isRightKeyPressed = input->isRightKeyPressed();
	bool isLeftKeyPressed = input->isLeftKeyPressed();
	bool isSpaceKeyTypePressed = input->getKeyState(SDL_SCANCODE_SPACE) == KeyboardState::PRESSED;
	switch (state) {
	case PlayerStateType::NON:
		break;

	case PlayerStateType::IDLE:
	case PlayerStateType::RUN: {
		if (isRightKeyPressed ^ isLeftKeyPressed) {
			setState(PlayerStateType::RUN);
			if (isRightKeyPressed)	setRightMoveSpeed();
			else					setLeftMoveSpeed();
		}
		else {
			setState(PlayerStateType::IDLE);
		}

		if (isSpaceKeyTypePressed) {
			setState(PlayerStateType::JUMP);
			setJumpSpeed();
			break;
		}

		//갑자기 바닥이 사라지는 등의 이벤트 발생을 의미
		if (isPlayerTouchedGround())
			ySpeed = 0;
		else {
			setState(PlayerStateType::FALL_JUMP);
			//ySpeed -= 1;
			break;
		}
	}
		break;

	case PlayerStateType::JUMP: {
		if (ySpeed < 0 &&
			state == PlayerStateType::JUMP)
			setState(PlayerStateType::FALL_JUMP);
	}
	case PlayerStateType::FALL_JUMP: {
		if (isSpaceKeyTypePressed) {
			setState(PlayerStateType::DOUBLE_JUMP);
			setJumpSpeed();
			break;
		}
	}
	case PlayerStateType::DOUBLE_JUMP: {
		if (((clock() - stateStartTime) / MotionPerSecond) > 6 &&
			state == PlayerStateType::DOUBLE_JUMP)
			setState(PlayerStateType::FALL_DOUBLE_JUMP);
	}
	case PlayerStateType::FALL_DOUBLE_JUMP: {
		if (isRightKeyPressed ^ isLeftKeyPressed) {
			if (isRightKeyPressed)
				setRightMoveSpeed();
			else
				setLeftMoveSpeed();
		}

		if (isPlayerTouchedGround()) {
			if (isRightKeyPressed ^ isLeftKeyPressed)
				setState(PlayerStateType::RUN);
			else
				setState(PlayerStateType::IDLE);
			break;
		}

		if (!isHeightEnoughToHangOnWall() || (!isPlayerTouchedOnWallRightSide() && !isPlayerTouchedOnWallLeftSide()));
		else if (isLeftKeyPressed && isPlayerTouchedOnWallRightSide() && isHeightEnoughToHangOnWall()) {
			setState(PlayerStateType::WALL_JUMP);
			flipState = FlipStateType::LOOK_AT_RIGHT_ON_WALL;
			xCoord = 0;
			xSpeed = 0;
		}
		else if (isRightKeyPressed && isPlayerTouchedOnWallLeftSide() && isHeightEnoughToHangOnWall()) {
			setState(PlayerStateType::WALL_JUMP);
			flipState = FlipStateType::LOOK_AT_LEFT_ON_WALL;
			xCoord = RenderEngine::ScreenWidth - TextureXYSize;
			xSpeed = 0;
		}
	}
		break;

	case PlayerStateType::WALL_JUMP: {
		if (!isHeightEnoughToHangOnWall() || (!isPlayerTouchedOnWallRightSide() && !isPlayerTouchedOnWallLeftSide())) {
			setState(PlayerStateType::FALL_JUMP);
		}

		if (isRightKeyPressed ^ isLeftKeyPressed) {
			if (flipState == FlipStateType::LOOK_AT_RIGHT_ON_WALL && isRightKeyPressed) {
				setState(PlayerStateType::FALL_JUMP);
				flipState = FlipStateType::LOOK_AT_RIGHT;
				setRightMoveSpeed();
				break;
			}
			if (flipState == FlipStateType::LOOK_AT_LEFT_ON_WALL && isLeftKeyPressed) {
				setState(PlayerStateType::FALL_JUMP);
				flipState = FlipStateType::LOOK_AT_LEFT;
				setLeftMoveSpeed();
				break;
			}
		}

		if (isSpaceKeyTypePressed) {
			if (flipState == FlipStateType::LOOK_AT_RIGHT_ON_WALL) {
				setState(PlayerStateType::JUMP);
				flipState = FlipStateType::LOOK_AT_RIGHT;
				setRightWallJumpSpeed();
			}
			if (flipState == FlipStateType::LOOK_AT_LEFT_ON_WALL) {
				setState(PlayerStateType::JUMP);
				flipState = FlipStateType::LOOK_AT_LEFT;
				setLeftWallJumpSpeed();
			}
		}
		else
			ySpeed = -1;
	}
		break;

	case PlayerStateType::HIT: {
		assert(false);
	}
		break;
	}

	yCoord += ySpeed;
	ySpeed -= 1;

	xCoord += xSpeed;
	if (xSpeed >= 1)		xSpeed -= 1;
	else if (xSpeed <= -1)	xSpeed += 1;
	else					xSpeed = 0;

	/*나중에 알아서 바닥면 충돌에 추가하는 방식으로 잘 일반화하기*/
	/* 바닥과 벽에 충돌했을 때, 블록에 끼이거나 통과하지 않도록 보정치 설정 */
	if (isPlayerTouchedOnWallRightSide()) xCoord = 0;
	if (isPlayerTouchedOnWallLeftSide()) xCoord = RenderEngine::ScreenWidth - TextureXYSize;

	if (yCoord < TextureXYSize - RenderEngine::ScreenHeight)
		yCoord = TextureXYSize - RenderEngine::ScreenHeight;
}
void Player::draw() {
	setTextureWithState();
	//SDL_Texture* texture = IMG_LoadTexture(renderer, "Pixel Adventure 1/Main Characters/Ninja Frog/Idle (32x32).png");
	//왜 이걸 지우면 제대로 된 출력이 되질 않는가?
	//몰라 뭐 renderer 참조에 문제있었나 보지

	short animationLength = 0;
	switch (state) {
	case PlayerStateType::IDLE:				animationLength = 11;	break;
	case PlayerStateType::RUN:				animationLength = 12;	break;
	case PlayerStateType::JUMP:				animationLength = 1;	break;
	case PlayerStateType::DOUBLE_JUMP:		animationLength = 6;	break;
	case PlayerStateType::WALL_JUMP:		animationLength = 5;	break;
	case PlayerStateType::FALL_JUMP:		animationLength = 1;	break;
	case PlayerStateType::FALL_DOUBLE_JUMP:	animationLength = 1;	break;
	case PlayerStateType::HIT:				animationLength = 7;	break;
	default:
		break;
	}
	SDL_Rect srcRect = {
		32 * getAnimationIdxWithTime(animationLength), 0,
		32, 32
	};
	SDL_Rect dstRect = {
		xCoord, -yCoord,
		TextureXYSize, TextureXYSize
	};
	SDL_RendererFlip sdlFlipType = SDL_FLIP_NONE;
	switch (flipState) {
	case FlipStateType::LOOK_AT_RIGHT:
	case FlipStateType::LOOK_AT_LEFT_ON_WALL:	sdlFlipType = SDL_FLIP_NONE;		break;
	case FlipStateType::LOOK_AT_LEFT:
	case FlipStateType::LOOK_AT_RIGHT_ON_WALL:	sdlFlipType = SDL_FLIP_HORIZONTAL;	break;
	default:
		sdlFlipType = SDL_FLIP_NONE;
		break;
	}
	SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0, nullptr, sdlFlipType);

	SDL_DestroyTexture(texture);
}
