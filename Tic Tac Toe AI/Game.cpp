#include <iostream>
#include <SDL_image.h>
#include "Game.h"

constexpr int ScreenWidth = 15 + 450 + 15 + 345 + 15;
constexpr int ScreenHeight = 60 + 450 + 60;

//init
void Game::resetField() {
	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			field[y][x] = DivideSign::NON;
		}
	}
}
//Game::init(), Game::update()에서만 호출됨
void Game::restartGame() {
	switch (firstTurn) {
	case DivideSign::NON:
	case DivideSign::COM: firstTurn = DivideSign::HUM; break;
	case DivideSign::HUM: firstTurn = DivideSign::COM; break;
	}
	turn = firstTurn;

	aiPlayer->readyNewGame(getWinner());

	resetField();
}
void Game::init() {
	aiPlayer = new AI();
	aiPlayer->init(this);
	resetField();
	restartGame();

	window = SDL_CreateWindow("Tic Tac Toe",
		100, 100,
		ScreenWidth, ScreenHeight, NULL);
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

//input
void Game::input() {
	SDL_Event event;
	if (mousePressType == MousePressType::RELEASED)
		mousePressType = MousePressType::COMMON;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				mouseXCoord = event.button.x;
				mouseYCoord = event.button.y;
				mousePressType = MousePressType::PRESSED;
			}
			break;
		case SDL_MOUSEMOTION:
			if (event.button.button == SDL_BUTTON_LEFT) {
				mouseXCoord = event.button.x;
				mouseYCoord = event.button.y;
				mousePressType = MousePressType::HOLDED;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				mouseXCoord = event.button.x;
				mouseYCoord = event.button.y;
				mousePressType = MousePressType::RELEASED;
			}
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		isRunning = false;
	}
}

//update
DivideSign Game::getWinner() {
	//어느 한 쪽의 사인이 3칸 연속으로 존재하는지 확인
	for (int y = 0; y < 3; ++y)
		if (field[y][0] != DivideSign::NON &&
			field[y][0] == field[y][1] &&
			field[y][1] == field[y][2])
			return field[y][0];
	for (int x = 0; x < 3; ++x)
		if (field[0][x] != DivideSign::NON &&
			field[0][x] == field[1][x] &&
			field[1][x] == field[2][x])
			return field[0][x];

	if (field[0][0] != DivideSign::NON &&
		field[0][0] == field[1][1] &&
		field[1][1] == field[2][2])
		return field[1][1];
	if (field[0][2] != DivideSign::NON &&
		field[0][2] == field[1][1] &&
		field[1][1] == field[2][0])
		return field[1][1];

	return DivideSign::NON;
}
bool Game::isGameDone() {
	//승패가 났을 때 역시 게임이 종료된다.
	if (getWinner() != DivideSign::NON)
		return true;

	//9칸이 모두 차서 더 이상의 게임 진행이 불가능한지 확인
	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			if (field[y][x] == DivideSign::NON)
				return false;
	return true;
}
void Game::playerTurnUpdate() {
	// x |  15~164 : 0, 165~314 : 1, 315~464 : 2
	// y | 155~304 : 0, 305~454 : 1, 455~604 : 2
	short xIdx = (mouseXCoord - 15 >= 0) ? (mouseXCoord - 15) / 150 : -1;
	short yIdx = (mouseYCoord - 60 >= 0) ? (mouseYCoord - 60) / 150 : -1;
	static Coord pressedButtonIdx = Coord(-1, -1);
	switch (mousePressType) {
	case MousePressType::PRESSED:
		if (0 <= xIdx && xIdx <= 2 &&
			0 <= yIdx && yIdx <= 2) {
			pressedButtonIdx = Coord(xIdx, yIdx);
			isButtonPressed = true;
		}
		else
			pressedButtonIdx = Coord(-1, -1);
		break;
	case MousePressType::HOLDED:
		if (pressedButtonIdx.x != xIdx ||
			pressedButtonIdx.y != yIdx) {
			pressedButtonIdx = Coord(-1, -1);
			isButtonPressed = false;
		}
		break;
	case MousePressType::RELEASED:
		if (0 <= xIdx && xIdx <= 2 &&
			0 <= yIdx && yIdx <= 2 &&
			pressedButtonIdx.x == xIdx &&
			pressedButtonIdx.y == yIdx &&
			field[yIdx][xIdx] == DivideSign::NON) {
			field[yIdx][xIdx] = DivideSign::HUM;
			turn = DivideSign::COM;
		}
		isButtonPressed = false;
		aiPlayer->addMovement();
		break;
	}
}
void Game::aiplayerTurnUpdate() {
	Coord aiSelection = aiPlayer->selectSpace();
	field[aiSelection.y][aiSelection.x] = DivideSign::COM;
	turn = DivideSign::HUM;
	aiPlayer->addMovement();
}
void Game::update() {
	if (isGameDone()) {
		//득점처리 및 결과 표시
		DivideSign winner = getWinner();
		switch (winner) {
		case DivideSign::COM: comScore++; break;
		case DivideSign::HUM: humScore++; break;
		case DivideSign::NON: drwScore++; break;
		}
		//새 게임 준비
		wait();
		restartGame();
	}
	else {
		switch (turn) {
		case DivideSign::HUM: playerTurnUpdate();	break;
		case DivideSign::COM: aiplayerTurnUpdate();	break;
		}
	}
}

//render
void Game::renderStr(const char* str, short rectX, short rectY) {
	//w*h = 35*50, 10개
	//판과의 거리를 d라고 할 때, 첫 번째 숫자 이미지의 위치는 { d+465, 110, 35, 50 }이다.
	//그리고 아마 d의 값은 (15+50)이 될 듯

	//w*h = 35*50
	SDL_Texture* numTexture = IMG_LoadTexture(renderer, "numImages.png");
	//w*h = 40*(50+10)
	SDL_Texture* abcTexture = IMG_LoadTexture(renderer, "abcImages.png");

	for (int i = 0, deltaX = 0; str[i] != NULL; ++i) {
		if ('0' <= str[i] && str[i] <= '9') {
			SDL_Rect srcRect = {
				35 * (str[i] - '0'), 0,
				35, 50
			};
			SDL_Rect dstRect = {
				rectX + deltaX, rectY,
				35, 50
			};
			SDL_RenderCopy(renderer, numTexture, &srcRect, &dstRect);
			deltaX += 35;
		}
		else if ('a' <= str[i] && str[i] <= 'z') {
			SDL_Rect srcRect = {
				40 * (str[i] - 'a'), 0,
				40, 60
			};
			SDL_Rect dstRect = {
				rectX + deltaX, rectY,
				40, 60
			};
			SDL_RenderCopy(renderer, abcTexture, &srcRect, &dstRect);
			deltaX += 40;
		}
		else if ('A' <= str[i] && str[i] <= 'Z') {
			SDL_Rect srcRect = {
				40 * (str[i] - 'A'), 0,
				40, 60
			};
			SDL_Rect dstRect = {
				rectX + deltaX, rectY,
				40, 60
			};
			SDL_RenderCopy(renderer, abcTexture, &srcRect, &dstRect);
			deltaX += 40;
		}
		else if (' ' == str[i]) {
			//deltaX += 30;
			deltaX += 40;
		}
	}

	SDL_DestroyTexture(numTexture);
	SDL_DestroyTexture(abcTexture);
}
void Game::render() {
	SDL_SetRenderDrawColor(renderer, 0xDC, 0xDC, 0xAA, 0xFF);
	SDL_RenderClear(renderer);

	//게임판 출력
	//그리기가 시작되는 사각형
	//xy(15,5), wh(150,150)
	SDL_Texture* OImage = IMG_LoadTexture(renderer, "tile_o.png");
	SDL_Texture* XImage = IMG_LoadTexture(renderer, "tile_x.png");
	SDL_Texture* NImage = IMG_LoadTexture(renderer, "tile_n.png");
	SDL_Texture* NPImage = IMG_LoadTexture(renderer, "tile_n_pressed.png");
	SDL_Rect fieldSrcRect = {
		0, 0, 100, 100
	};
	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			SDL_Rect fieldDstRect = {
				15 + 150 * x, 60 + 150 * y,
				150, 150
			};
			switch (field[y][x]) {
			case DivideSign::COM: SDL_RenderCopy(renderer, XImage, &fieldSrcRect, &fieldDstRect); break;
			case DivideSign::HUM: SDL_RenderCopy(renderer, OImage, &fieldSrcRect, &fieldDstRect); break;
			case DivideSign::NON:
				const short xIdx = (mouseXCoord - 15) / 150;
				const short yIdx = (mouseYCoord - 60) / 150;
				if (isButtonPressed &&
					xIdx == x && yIdx == y)	SDL_RenderCopy(renderer, NPImage, &fieldSrcRect, &fieldDstRect);
				else						SDL_RenderCopy(renderer, NImage, &fieldSrcRect, &fieldDstRect);
				break;
			}
		}
	}
	SDL_DestroyTexture(OImage);
	SDL_DestroyTexture(XImage);
	SDL_DestroyTexture(NImage);
	SDL_DestroyTexture(NPImage);

	//턴 알림 출력
	SDL_Texture* ComTurnImage = IMG_LoadTexture(renderer, "turn_com.png");
	SDL_Texture* HumTurnImage = IMG_LoadTexture(renderer, "turn_hum.png");
	SDL_Texture* NonTurnImage = IMG_LoadTexture(renderer, "turn_non.png");
	SDL_Rect turnSrcRect = {
		0, 0, 100, 50
	};
	SDL_Rect comturnDstRect = {
		240 - 50, 5,
		100, 50
	};
	SDL_Rect humturnDstRect = {
		240 - 50, 515,
		100, 50
	};
	SDL_RenderCopy(renderer, NonTurnImage, &turnSrcRect, &comturnDstRect);
	SDL_RenderCopy(renderer, NonTurnImage, &turnSrcRect, &humturnDstRect);
	switch (turn) {
	case DivideSign::COM:	SDL_RenderCopy(renderer, HumTurnImage, &turnSrcRect, &comturnDstRect); break;
	case DivideSign::HUM:	SDL_RenderCopy(renderer, ComTurnImage, &turnSrcRect, &humturnDstRect); break;
	}
	SDL_DestroyTexture(ComTurnImage);
	SDL_DestroyTexture(HumTurnImage);
	SDL_DestroyTexture(NonTurnImage);

	//점수 출력
	char buff[20] = {};
	sprintf_s(buff, "COM  %-4d", comScore);	renderStr(buff, 480, 110 + 150 * 0);
	sprintf_s(buff, "DRAW %-4d", drwScore);	renderStr(buff, 480, 110 + 150 * 1);
	sprintf_s(buff, "HUM  %-4d", humScore);	renderStr(buff, 480, 110 + 150 * 2);

	SDL_RenderPresent(renderer);
}

//release
void Game::release() {
	aiPlayer->release();
}

//systems
void Game::wait(int time) {
	clock_t oldClock = clock();
	while (oldClock + time > clock());
}
void Game::wait() {
	int time = 350;
	wait(time);
}
void Game::run() {
	init();
	while (isRunning) {
		input();
		update();
		render();
	}
	release();
}
