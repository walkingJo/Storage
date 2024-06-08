#include "Game.h"

void Game::init() {
	renderer = RenderEngine();
	renderer.init();

	platform = PlatformManager(renderer.getRenderer());
	platform.loadPlatformDataFromCsvFile("null");

	input = InputProcessor();
	isRunning = true;

	playerObj = Player(CharacterType::NINJA_FROG, renderer.getRenderer(), &platform);
	platform.setPlayerCoordOnStartPoint(&playerObj);

	time = clock();
	deltaTime = 1000 / 60;
}
void Game::getInput() {
	input.update();
	isRunning = input.isRunning();

/**/if (input.getKeyState(SDL_SCANCODE_R) == KeyboardState::PRESSED)
/**/	platform.setPlayerCoordOnStartPoint(&playerObj);

/**/if (input.getKeyState(SDL_SCANCODE_P) == KeyboardState::PRESSED)
/**/	switch (playerObj.getState()) {
/**/	case PlayerStateType::NON:				printf("non\n");				break;
/**/	case PlayerStateType::IDLE:				printf("idle\n");				break;
/**/	case PlayerStateType::RUN:				printf("run\n");				break;
/**/	case PlayerStateType::JUMP:				printf("jump\n");				break;
/**/	case PlayerStateType::DOUBLE_JUMP:		printf("double jump\n");		break;
/**/	case PlayerStateType::WALL_JUMP:		printf("wall jump\n");			break;
/**/	case PlayerStateType::FALL_JUMP:		printf("fall jump\n");			break;
/**/	case PlayerStateType::FALL_DOUBLE_JUMP:	printf("fall double jump\n");	break;
/**/	case PlayerStateType::HIT:				printf("hit\n");				break;
/**/	}
}
void Game::update() {
	playerObj.update(&input);
}
void Game::render() {
	renderer.renderClear();

	platform.draw();
	playerObj.draw();

	renderer.renderPresent();
}
void Game::wait() {
	clock_t oldClock = clock();
	while (1000.0f / 60 < clock() - oldClock);
	deltaTime = clock() - oldClock;
}
void Game::release() {
	renderer.release();
	SDL_Quit();
}

Game::Game() {
	init();
}
void Game::run() {
	while (isRunning) {
		getInput();
		update();
		render();
		wait();
	}
	release();
}
