#include <vector>
#include "Game.h"

Game::Game() {
	player = Object(this);
	polygon1 = Object(this);
	polygon2 = Object(this);
	polygon3 = Object(this);

	clock = 0;
	deltaTime = 0.01f;
	isRunning = true;
	printf("# Fields of game ready\n");
}
Game::~Game() {
	return;
}

void Game::initialize() {
	fieldInit();
	windowInit();
}
void Game::fieldInit() {
	renderer = RenderEngine(this);

	player.moveTo(Vector3(0, 0, 1000));
	player.setModel(Model::Icosahedron);
	player.scale(50);

	polygon1.moveTo(Vector3(-150, 0, 1000));
	polygon1.setModel(Model::Tetrahedron);
	polygon1.scale(50);

	polygon2.moveTo(Vector3(+150, 0, 1000));
	//polygon2.setModel(Model::Octahedron);
	polygon2.setModel(Model::Sphere);
	polygon2.scale(50);

	polygon3.moveTo(Vector3(0, 150, 1000));
	polygon3.setModel(Model::Tetrahedron);
	polygon3.direction = Vector3::UnitY;
	polygon3.scale(50);

	//renderer.addObject(player);
	//renderer.addObject(polygon1);
	//renderer.addObject(polygon2);
	//renderer.addObject(polygon3);

	clock = SDL_GetTicks();
	printf("# Game objects ready...\n");
}
void Game::windowInit() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		throw;
	}
	printf("# SDL_Init done...\n");

	/*if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		throw;
	}
	printf("# IMG_Init done...\n");
	*/

	renderer.createWindow();

	printf("\n");
	printf("-- All processes done --\n");
	printf("    -- Game start --\n");
}

void Game::processInput() {
	SDL_Event event;
	static int mouseNewMotionX{ 0 }, mouseOldMotionX{ 0 };
	static int mouseNewMotionY{ 0 }, mouseOldMotionY{ 0 };
	static bool isMousePressed = false;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouseNewMotionX = mouseOldMotionX = event.button.x;
			mouseNewMotionY = mouseOldMotionY = event.button.y;
			isMousePressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			isMousePressed = false;
			break;
		case SDL_MOUSEMOTION:
			if (isMousePressed) {
				mouseNewMotionX = event.motion.x;
				mouseNewMotionY = event.motion.y;
			}
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		isRunning = false;
	}

	// 함수로 분리해서 Object에 Rotate로 삽입
	/**/ renderer.camera.processInput(state);
	/**/ renderer.camera.rotate(Vector3::UnitY, -Math::ToRadians((float)mouseNewMotionX - mouseOldMotionX) / 15);
	/**/ renderer.camera.rotate(Vector3::UnitX, Math::ToRadians((float)mouseNewMotionY - mouseOldMotionY) / 15);
	/**/ mouseOldMotionX = mouseNewMotionX;
	/**/ mouseOldMotionY = mouseNewMotionY;
}

void Game::update() {
	physicsUpdate();
	renderUpdate();
	timeUpdate();
}
void Game::physicsUpdate() {
	Vector3 playerAxis = Vector3::UnitY;
	Vector3 polygon1Axis = Vector3::Normalize(Vector3(+1, 1, 0));
	Vector3 polygon2Axis = Vector3::Normalize(Vector3(-1, 1, 0));
	Vector3 polygon3Axis = Vector3::Normalize(Vector3(1, 1, 1));

	player.rotate(playerAxis, Math::Pi / 60);
	polygon1.rotate(polygon1Axis, Math::Pi / 60);
	polygon2.rotate(polygon2Axis, Math::Pi / 60);
	polygon3.lookAt(renderer.camera.location);
	//polygon3.Rotate(polygon3Axis, Math::Pi / 60);
	//polygon3.rotateBySelf(Vector3::NegUnitX, Math::Pi / 2);
}
void Game::renderUpdate() {
	renderer.clear(0, 255, 0);

	// Draw all sprite components
	renderer.drawObject(player);
	renderer.drawObject(polygon1);
	renderer.drawObject(polygon2);
	renderer.drawObject(polygon3);
	//renderer.drawAllObject();

	renderer.renderPresent();
}
void Game::timeUpdate() {
	while (deltaTime < 0.01f) {
		deltaTime = (SDL_GetTicks() - clock) / 1000.0f;
	}
	clock = SDL_GetTicks();
}

void Game::release() {
	printf("\n");

	renderer.~RenderEngine();
	printf("# renderer released...\n");
	
	/*IMG_Quit(); */
	
	SDL_Quit();
	printf("# SDL released...\n");
	
	this->~Game();
	printf("# Game released...\n");
	
	printf("# All process done...\n");
}
