#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <SDL.h>
#include <SDL_image.h>

constexpr float PI = 3.14159265358979323846f;

const char* title = "test project";
constexpr int ScreenWidth = 1270;
constexpr int ScreenHeight = 900;

constexpr char MaxFPS = 60;
constexpr int MaxObjects = 20;

constexpr float AngleAcceleration = 0.0125f;

const char numberDrawPoint[10][5][4]{
			{
				"###",
				"# #",
				"# #",
				"# #",
				"###",
			}, //0
			{
				" # ",
				"## ",
				" # ",
				" # ",
				"###",
			}, //1
			{
				"###",
				"  #",
				"###",
				"#  ",
				"###",
			}, //2
			{
				"###",
				"  #",
				"###",
				"  #",
				"###",
			}, //3
			{
				"# #",
				"# #",
				"###",
				"  #",
				"  #",
			}, //4
			{
				"###",
				"#  ",
				"###",
				"  #",
				"###",
			}, //5
			{
				"###",
				"#  ",
				"###",
				"# #",
				"###",
			}, //6
			{
				"###",
				"# #",
				"# #",
				"  #",
				"  #",
			}, //7
			{
				"###",
				"# #",
				"###",
				"# #",
				"###",
			}, //8
			{
				"###",
				"# #",
				"###",
				"  #",
				"###",
			}, //9
};

class MouseCoord {
public:
	int x, y;

	MouseCoord() : x(0), y(0) {}
	MouseCoord(int x, int y) : x(x), y(y) {}
};

class RGBA {
public:
	unsigned char r, g, b, a;

	RGBA() { r = g = b = a = 255; }
	RGBA(unsigned r, unsigned g,
		unsigned b, unsigned a = 255) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

float getAngleSpeed(float deltaTime) {
	return 750.f * (0.679106854741f + (rand() % 1000 / 1500.0f)) /
		((float)deltaTime + 0.5f);
}

void stretchTextureEx(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Texture* texture,
	float angle, SDL_RendererFlip flip = SDL_FLIP_NONE) {
	SDL_Rect src, dst;
	SDL_Point center;

	src.x = src.y = 0;
	SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;

	center.x = w / 2;
	center.y = h / 2;

	SDL_RenderCopyEx(renderer, texture, &src, &dst, angle, &center, flip);
}


int main(int argc, char* argv[]) {
#pragma region Fields
#pragma region Fields::System
	SDL_Window* window;
	SDL_Renderer* renderer;
	const RGBA clearColor = RGBA(0xDC, 0xDC, 0xAA);

	bool isRunning = true;
	MouseCoord newCoord(0, 0), oldCoord(0, 0);
	// 0 : 기본
	// 1 : 눌림
	// 2 : 드래그
	// 3 : 떼짐
	char mousePressType = 0;
	bool isMouseMoved = false;
	const Uint8* keyState = nullptr;

	clock_t oldTime;
#pragma endregion
#pragma region Fields::Common
	bool isSpinning = false;
	//-1 : 왼쪽 눌림
	// 0 : 기본
	// 1 : 오른쪽 눌림
	char increaseButtonClickType = 0;

	int objectCount = 7;
	float objectAngle[MaxObjects] = {};

	int srcYCoord = 300;

	bool isHandleClicked = false;
	float angleSpeed = 0.0f;

	SDL_Texture* texture;
#pragma endregion
#pragma endregion

#pragma region Init
	srand((unsigned int)time(NULL));
	oldTime = clock();

	//objectAngle init
	for (int i = 0; i < MaxObjects; ++i) {
		objectAngle[i] = 0.0f;
	}

#pragma region Window::Init
	window = SDL_CreateWindow(title, 100, 100, ScreenWidth, ScreenHeight, 0);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return -1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return -1;
	}


	texture = IMG_LoadTexture(renderer, "Disk7.png");
#pragma endregion

	//class Event
#pragma endregion

	clock_t clickedTime = clock();
	while (isRunning) {
#pragma region GetInput
		oldCoord = newCoord;
		if (mousePressType == 3) mousePressType = 0;
		isMouseMoved = false;
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button != SDL_BUTTON_LEFT) break;
				mousePressType = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button != SDL_BUTTON_LEFT) break;
				mousePressType = 3;
				break;
			case SDL_MOUSEMOTION:
				newCoord = MouseCoord(event.button.x, event.button.y);
				if (mousePressType == 1) mousePressType = 2;
				isMouseMoved = true;
				break;
			}
		}
		keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_ESCAPE]) {
			isRunning = false;
		}
#pragma endregion
#pragma region Update
#pragma region Update::Handle::Clicked
		//(1000-120, 300)~(1000+120, 750)
		//(880, 300)~(1120, 750)

		switch (mousePressType) {
		case 0: break;
		case 1:
			if (!isSpinning &&
				880 <= newCoord.x && newCoord.x < 1120 &&
				300 <= newCoord.y && newCoord.y < 350) {
				clickedTime = clock();

				isSpinning = false;
				srcYCoord = newCoord.y;

				isHandleClicked = true;
			}
			else if (isSpinning &&
				880 <= newCoord.x && newCoord.x < 1120 &&
				625 <= newCoord.y && newCoord.y < 675) {
				isSpinning = false;
				srcYCoord = 350;
				isHandleClicked = false;
			}
			break;
		case 2:
			if (isHandleClicked && 325 <= newCoord.y - srcYCoord) {
				clock_t deltaTime = clock() - clickedTime;

				isSpinning = true;
				srcYCoord = 350;

				isHandleClicked = false;
				angleSpeed = getAngleSpeed((float)deltaTime);
			}
			break;
		case 3:
			srcYCoord = 350;
			isHandleClicked = false;
			break;
		}
#pragma endregion
		if (isSpinning) {
#pragma region Update::Disk::Spin
			for (int i = 0; i < objectCount; ++i) {
				objectAngle[i] += angleSpeed;
			}
			angleSpeed -= AngleAcceleration;
			
			if (angleSpeed < 0) {
				angleSpeed = 0;
				isSpinning = false;
				//triEvent.start();
			}
		}
#pragma endregion
		else {
			//triEvent.update();
			//triEvent.render();
#pragma region Update::Disk::Set Image
			if (increaseButtonClickType != 0) {
				float angle_0 = 2 * PI / objectCount;
				for (int i = 0; i < objectCount; ++i) {
					objectAngle[i] = i * angle_0;
				}
			}
#pragma endregion
#pragma region Update::Button Clicked
			increaseButtonClickType = 0;
			if (880 <= newCoord.x && newCoord.x < 930 &&
				150 <= newCoord.y && newCoord.y < 200) {
				switch (mousePressType) {
				case 0: break;
				case 1:
				case 2: increaseButtonClickType = -1;	break;
				case 3: increaseButtonClickType = -1;
					if (objectCount > 2)
						objectCount--;
					break;
				}
			}
			if (1070 <= newCoord.x && newCoord.x < 1120 &&
				 150 <= newCoord.y && newCoord.y < 200) {
				switch (mousePressType) {
				case 0: break;
				case 1:
				case 2: increaseButtonClickType = 1;	break;
				case 3: increaseButtonClickType = 1;
					if (objectCount < MaxObjects)
						objectCount++;
					break;
				}
			}
#pragma endregion
		}
#pragma endregion
#pragma region Render
#pragma region Render::Clear
		SDL_SetRenderDrawColor(renderer,
			clearColor.r, clearColor.g,
			clearColor.b, clearColor.a);
		SDL_RenderClear(renderer);
#pragma endregion
#pragma region Render::Draw::Disk
		int centorXY = 450;

		stretchTextureEx(renderer, 450 - 300, 450 - 300, 600, 600, texture, objectAngle[0]);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		for (int x = -303; x <= +303; ++x) {
			for (int y = -303; y <= +303; ++y) {
				int lengthSq = x * x + y * y;
				if (88209 <= lengthSq && lengthSq <= 91809)
					SDL_RenderDrawPoint(renderer, x + 450, y + 450);
			}
		}

		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//for (int x = -300; x <= 300; ++x) {
		//	for (int y = -300; y <= 300; ++y) {
		//		int distanceSq = x * x + y * y;
		//		if (22500 <= distanceSq &&distanceSq < 90000) {
		//			//set color
		//			SDL_RenderDrawPoint(renderer, x + centorXY, y + centorXY);
		//		}
		//	}
		//}

		//for (int i = 0; i < objectCount; ++i) {
		//	SDL_SetRenderDrawColor(renderer,
		//		255 * i / objectCount,
		//		0,
		//		255 - 255 * i / objectCount, 255);
		//	SDL_Rect tempRect = {
		//		+(int)(300 * sin(objectAngle[i])) + centorXY - 10,
		//		-(int)(300 * cos(objectAngle[i])) + centorXY - 10,
		//		20,
		//		20
		//	};
		//	SDL_RenderDrawRect(renderer, &tempRect);
		//}

		const SDL_Vertex vertices[3] = {
			{ { (float)centorXY + 15, (float)centorXY - 320 }, { 128, 0, 0 }, {0} },
			{ { (float)centorXY - 15, (float)centorXY - 320 }, { 128, 0, 0 }, {0} },
			{ { (float)centorXY + 00, (float)centorXY - 280 }, { 128, 0, 0 }, {0} }
		};
		const int indices[3] = {
			0, 1, 2
		};
		SDL_RenderGeometry(renderer, nullptr, vertices, 3, indices, 3);
#pragma endregion
#pragma region Render::Draw::Handle
		//(1000-120, 300)~(1000+120, 750)
		//(880, 300)~(1120, 750)
		//하단 75, 상단 375(손잡이 50)

		int handleYCoord;
		if (isHandleClicked)	handleYCoord = 325 + newCoord.y - srcYCoord;
		else					handleYCoord = 325;
		if (handleYCoord < 325) handleYCoord = 325;
		else if (handleYCoord > 650) handleYCoord = 650;

		if (isSpinning) handleYCoord = 650; //핸들이 아래에 고정되도록 하는 코드

		SDL_Rect handleRect = {
			910, handleYCoord - 25,
			180, 50
		};
		SDL_SetRenderDrawColor(renderer, 192, 0, 0, 255);
		SDL_RenderFillRect(renderer, &handleRect);

		SDL_Rect handleBottomRect = {
			880, 675,
			240, 75
		};
		SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
		SDL_RenderFillRect(renderer, &handleBottomRect);

		SDL_Rect handleHolderRect = {
			1000 - 15, handleYCoord + 25,
			30,        675 - (handleYCoord + 25)
		};
		SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
		SDL_RenderFillRect(renderer, &handleHolderRect);

		const SDL_Vertex shineVertices[6] = {
			{ { (float)handleRect.x + 10, (float)handleRect.y + 10 }, { 255, 120, 120 }, {0} },
			{ { (float)handleRect.x + 80, (float)handleRect.y + 10 }, { 255, 120, 120 }, {0} },
			{ { (float)handleRect.x + 10, (float)handleRect.y + 25 }, { 255, 120, 120 }, {0} },

			{ { (float)handleBottomRect.x + 10, (float)handleBottomRect.y + 10 }, { 60, 60, 60 }, {0} },
			{ { (float)handleBottomRect.x +140, (float)handleBottomRect.y + 10 }, { 60, 60, 60 }, {0} },
			{ { (float)handleBottomRect.x + 10, (float)handleBottomRect.y + 50 }, { 60, 60, 60 }, {0} },
		};
		const int shineIndices[6] = {
			0, 1, 2,
			3, 4, 5
		};
		SDL_RenderGeometry(renderer, NULL, shineVertices, 6, shineIndices, 6);
#pragma endregion
#pragma region Render::Draw::Buttons
		RGBA onColor = RGBA(50, 50, 50);
		RGBA offColor = RGBA(120, 120, 120);
		
		//draw left button
		if (isSpinning ||
			increaseButtonClickType == -1)	SDL_SetRenderDrawColor(renderer, onColor.r, onColor.g, onColor.b, 255);
		else								SDL_SetRenderDrawColor(renderer, offColor.r, offColor.g, offColor.b, 255);
		//button body
		for (int x = 880; x < 930; ++x) {
			for (int y = 150; y < 200; ++y)
				SDL_RenderDrawPoint(renderer, x, y);
		}
		//'-'
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		for (int x = 890; x <= 920; ++x) for (int y = 170; y <= 180; ++y) SDL_RenderDrawPoint(renderer, x, y);
		
		//draw right button
		if (isSpinning ||
			increaseButtonClickType == 1)	SDL_SetRenderDrawColor(renderer, onColor.r, onColor.g, onColor.b, 255);
		else								SDL_SetRenderDrawColor(renderer, offColor.r, offColor.g, offColor.b, 255);
		//button body
		for (int x = 1070; x < 1120; ++x) {
			for (int y = 150; y < 200; ++y)
				SDL_RenderDrawPoint(renderer, x, y);
		}
		//'+'
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		for (int x = 1080; x <= 1110; ++x) for (int y = 170; y <= 180; ++y) SDL_RenderDrawPoint(renderer, x, y);
		for (int x = 1090; x <= 1100; ++x) for (int y = 160; y <= 190; ++y) SDL_RenderDrawPoint(renderer, x, y);

#pragma endregion
#pragma region Render::Draw::Number
		char num01 = objectCount % 10;
		char num10 = objectCount / 10;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		// 10 | x : 1017~1047
		for (int h = 0; h < 5; ++h) for (int w = 0; w < 3; ++w)
			if (numberDrawPoint[num10][h][w] != ' ')
				for (int x = 967; x < 977; ++x) for (int y = 150; y < 160; ++y)
					SDL_RenderDrawPoint(renderer, x + 10 * w, y + 10 * h);
		// 01 | x : 1053~1083
		for (int h = 0; h < 5; ++h) for (int w = 0; w < 3; ++w)
			if (numberDrawPoint[num01][h][w] != ' ')
				for (int x = 1003; x < 1013; ++x) for (int y = 150; y < 160; ++y)
					SDL_RenderDrawPoint(renderer, x + 10 * w, y + 10 * h);
#pragma endregion
#pragma region Render::Present
		SDL_RenderPresent(renderer);
#pragma endregion
#pragma endregion
#pragma region Wait
		while (clock() - oldTime < 1000.0f / MaxFPS);
		oldTime = clock();
#pragma endregion
	}

#pragma region Release
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
#pragma endregion

	return 0;
}
