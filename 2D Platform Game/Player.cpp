#include <SDL_image.h>
#include <string>
#include <cassert>
#include <ctime>
#include "RenderEngine.h"
#include "InputProcessor.h"
#include "Player.h"

constexpr int MotionPerSecond = 1000 / 15;
constexpr int moveCoord = 10;
constexpr int JumpSpeed = 15;
constexpr int WallJumpXSpeed = 13;
constexpr int WallJumpYSpeed = 15;

constexpr int marginXYSize = 10;

void Player::setTextureWithState() {
	//		"Pixel Adventure 1/Main Characters/Player Type/File Name.png";
	//		                                   ^^^^^^^^^^^ ^^^^^^^^^
	if (texture != NULL)
		SDL_DestroyTexture(texture);
	if (renderer == NULL)
		return;

	std::string characterNames[4] = {
		"Mask Dude",
		"Virtual Guy",
		"Pink Man",
		"Ninja Frog",
	};
	std::string fileNames[7] = {
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

//바닥에 닿는 판정은 일반적으로 벽에 닿는 판정을 (양쪽 모두) 포함한다.
bool Player::isPlayerTouchedOnWallRightSide() {
	if (platform->isPlatformExist(xCoord + marginXYSize - 1, yCoord - marginXYSize))
		return true;
	else
		return false;
}
bool Player::isPlayerTouchedOnWallLeftSide() {
	if (platform->isPlatformExist(xCoord - marginXYSize + PlayerTextureXYSize, yCoord - marginXYSize))
		return true;
	else
		return false;
}
bool Player::isPlayerTouchedGround() {
	if (platform->isPlatformExist(xCoord + marginXYSize - 1 + 1, yCoord - PlayerTextureXYSize) ||
		platform->isPlatformExist(xCoord - marginXYSize + PlayerTextureXYSize - 1, yCoord - PlayerTextureXYSize))
		return true;
	else
		return false;
}
bool Player::isPlayerTouchedOnCeiling() {
	if (platform->isPlatformExist(xCoord + marginXYSize - 1 + 1, yCoord - marginXYSize) ||
		platform->isPlatformExist(xCoord - marginXYSize + PlayerTextureXYSize - 1, yCoord - marginXYSize))
		return true;
	else
		return false;
}

void Player::setRightMoveSpeed() {
	xCoord += moveCoord;
	flipState = FlipStateType::LOOK_AT_RIGHT;
}
void Player::setLeftMoveSpeed() {
	xCoord -= moveCoord;
	flipState = FlipStateType::LOOK_AT_LEFT;
}
void Player::setJumpSpeed() {
	ySpeed = JumpSpeed;
}
void Player::setRightWallJumpSpeed() {
	xSpeed = WallJumpXSpeed;
	ySpeed = WallJumpYSpeed;
}
void Player::setLeftWallJumpSpeed() {
	xSpeed = -WallJumpXSpeed;
	ySpeed = WallJumpYSpeed;
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
			setState(PlayerStateType::RUN);		//상태 설정 계수와 관련::움직임
			if (isRightKeyPressed)	setRightMoveSpeed();
			else					setLeftMoveSpeed();
		}
		else {
			setState(PlayerStateType::IDLE);	//상태 설정 계수와 관련::정지
		}

		if (isSpaceKeyTypePressed) {
			setState(PlayerStateType::JUMP);	//상태 설정 계수와 관련
			setJumpSpeed();
			break;
		}

		//갑자기 바닥이 사라지는 등의 이벤트 발생을 의미
		if (isPlayerTouchedGround()) {
			ySpeed = 0;
		}
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
			setState(PlayerStateType::DOUBLE_JUMP);	//상태 설정 계수와 관련
			setJumpSpeed();
			break;
		}
	}
	case PlayerStateType::DOUBLE_JUMP: {
		if (state == PlayerStateType::DOUBLE_JUMP &&
			((clock() - stateStartTime) / MotionPerSecond) > 6)
			setState(PlayerStateType::FALL_DOUBLE_JUMP);
	}
	case PlayerStateType::FALL_DOUBLE_JUMP: {
		if (isRightKeyPressed ^ isLeftKeyPressed) {
			if (isRightKeyPressed)	setRightMoveSpeed();
			else					setLeftMoveSpeed();
		}

		if (isPlayerTouchedGround() &&
			!isPlayerTouchedOnWallRightSide() &&
			!isPlayerTouchedOnWallLeftSide()) {
			if (isRightKeyPressed ^ isLeftKeyPressed)
				setState(PlayerStateType::RUN);
			else
				setState(PlayerStateType::IDLE);
			break;
		}
		else if (isLeftKeyPressed && isPlayerTouchedOnWallRightSide()) {
			setState(PlayerStateType::WALL_JUMP);
			flipState = FlipStateType::LOOK_AT_RIGHT_ON_WALL;
			xSpeed = 0;
		}
		else if (isRightKeyPressed && isPlayerTouchedOnWallLeftSide()) {
			setState(PlayerStateType::WALL_JUMP);
			flipState = FlipStateType::LOOK_AT_LEFT_ON_WALL;
			xSpeed = 0;
		}
	}
		break;

	case PlayerStateType::WALL_JUMP: {
		if (isPlayerTouchedGround() /*&&
			!isPlayerTouchedOnWallRightSide() && !isPlayerTouchedOnWallLeftSide()*/) {
			if (isRightKeyPressed ^ isLeftKeyPressed)
				setState(PlayerStateType::RUN);
			else
				setState(PlayerStateType::IDLE);
			break;
		}
		else if (!isPlayerTouchedOnWallRightSide() && !isPlayerTouchedOnWallLeftSide()) {
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
		else {
			ySpeed = -1;
		}
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

	//아래 함수들은 좌표값이 0을 지나갈 때 문제가 생기는 듯 하다.
	//지금의 움직임 범위에서는 문제가 없으니 카메라 기능을 구현한 후 현상을 관찰하자.
	if (isPlayerTouchedOnWallRightSide()) {
		xCoord += (PlayerTextureXYSize - (xCoord + marginXYSize) % PlayerTextureXYSize) % PlayerTextureXYSize;
		xSpeed = 0;
	}
	if (isPlayerTouchedOnWallLeftSide()) {
		xCoord -= (PlayerTextureXYSize + (xCoord - marginXYSize) % PlayerTextureXYSize) % PlayerTextureXYSize;
		xSpeed = 0;
	}
	if (isPlayerTouchedGround()) {
		yCoord += (PlayerTextureXYSize - yCoord % PlayerTextureXYSize) % PlayerTextureXYSize;
		ySpeed = 0;
	}
	if (isPlayerTouchedOnCeiling()) {
		yCoord -= (PlayerTextureXYSize + (yCoord - marginXYSize) % PlayerTextureXYSize) % PlayerTextureXYSize;
		ySpeed = 0;
	}
}
void Player::draw(int screenXCoord, int screenYCoord) {
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
		screenXCoord, screenYCoord,
		PlayerTextureXYSize, PlayerTextureXYSize
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
void Player::drawWithCameraCoord(int centorXCoord, int centorYCoord) {
	draw(
		xCoord - (centorXCoord - RenderEngine::centorXCoord),
		(centorYCoord - RenderEngine::centorYCoord) - yCoord
	);
}
