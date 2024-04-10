#include "Game.h"
#include "RenderEngine.h"

RenderEngine::RenderEngine(class Game* game) {
	window = nullptr;
	renderer = nullptr;
	//depthBuffer = new float* [ScreenHeight];
	//for (int i = 0; i < ScreenHeight; ++i)
	//	depthBuffer[i] = new float[ScreenWidth];
	this->game = game;
	camera = Object(game);
	objects = std::vector<Object>();
}
RenderEngine::RenderEngine() {
	RenderEngine(0);
}
RenderEngine::~RenderEngine() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//for (int i = 0; i < ScreenHeight; ++i) {
	//	delete depthBuffer[i];
	//}
	//delete depthBuffer;
}

void RenderEngine::createWindow() {
	window = SDL_CreateWindow("Project Name", 0, -1, ScreenWidth, ScreenHeight, 0);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		throw;
	}
	printf("# SDL_Window ready...\n");

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		throw;
	}
	printf("# SDL_Renderer ready...\n");
}

void RenderEngine::clear(int r, int g, int b) {
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderClear(renderer);
#pragma region depthBuffer
	for (int h = 0; h < ScreenHeight; ++h) {
		for (int w = 0; w < ScreenWidth; ++w) {
			depthBuffer[h][w] = ScreenFar;	
		}
	}
#pragma endregion
}
#pragma region draw
void RenderEngine::drawObject(Object object) {
	drawModel(object.getModel());
}
void RenderEngine::addObject(Object object) {
	objects.push_back(object);
}
void RenderEngine::drawAllObject() {
	for (int i = 0; i < objects.size(); ++i) {
		int max = i;
		for (int j = i + 1; j < objects.size(); ++i) {
			if ((objects[max].location - camera.location).LengthSq() < (objects[j].location - camera.location).LengthSq())
				max = j;
		}
		Object& temp = objects[max];
		objects[max] = objects[i];
		objects[i] = temp;
	}
	for (Object obj : objects)
		drawObject(obj);
}

void RenderEngine::drawModel(Model model) {
	model.sortByDepth(camera.location);
	for (int i : model.sortedIndices) {
		const Vector3& v1 = model.vertices[model.indices[3 * i + 0]];
		const Vector3& v2 = model.vertices[model.indices[3 * i + 1]];
		const Vector3& v3 = model.vertices[model.indices[3 * i + 2]];

		if (isV3RenderOk(v1) && isV3RenderOk(v1) && isV3RenderOk(v1)) {
			drawTriangleOnCamera(v1, v2, v3, SDL_Color{ 255, 255, 255, 255 });
			drawLineOnCamera(v1, v2, SDL_Color{ 0, 0, 0, 255 });
			drawLineOnCamera(v2, v3, SDL_Color{ 0, 0, 0, 255 });
			drawLineOnCamera(v3, v1, SDL_Color{ 0, 0, 0, 255 });
		}
	}

	//for (int i = 0; i < model.indices.size() / 3; ++i) {
	//	const Vector3& v1 = basisConversionWithCamera(model.vertices[model.indices[3 * i + 0]]);
	//	const Vector3& v2 = basisConversionWithCamera(model.vertices[model.indices[3 * i + 1]]);
	//	const Vector3& v3 = basisConversionWithCamera(model.vertices[model.indices[3 * i + 2]]);
	//
	//	drawTriangleOnCamera(v1, v2, v3, SDL_Color{ 255, 255, 255, 255 });
	//	drawLineOnCamera(v1, v2, SDL_Color{ 0, 0, 0, 255 });
	//	drawLineOnCamera(v2, v3, SDL_Color{ 0, 0, 0, 255 });
	//	drawLineOnCamera(v3, v1, SDL_Color{ 0, 0, 0, 255 });
	//}
}
void RenderEngine::renderPresent() {
	SDL_RenderPresent(renderer);
}

Vector3 RenderEngine::basisConversionWithCamera(Vector3 v) {
	v -= camera.location;
	return Vector3::Transform(v, camera.getBasisConversion());
}
// 다음 두 함수는 깊이 버퍼를 사용한 형식에서 사용되지 않는다
/**/ bool RenderEngine::isV3RenderOk(Vector3 v) {
	Vector3 changedV = basisConversionWithCamera(v);
	if (ScreenNear <= changedV.z && changedV.z <= ScreenFar)
		return true;
	else
		return false;
}
/**/ SDL_FPoint RenderEngine::vector3ToFPoint(Vector3 v) {
	float near = ScreenHeight * Math::Sqrt(3);

	v = basisConversionWithCamera(v);

	SDL_FPoint temp = {};
	temp.x = ScreenWidth / 2 + v.x * near / v.z;
	temp.y = ScreenHeight / 2 - v.y * near / v.z;
	return temp;
}

/*// input transformed Vector3*/
// 아래 두 함수들을 깊이 버퍼를 적용하여 수정
/**/void RenderEngine::drawPointOnCamera(Vector3 v1, SDL_Color color) {
	float nearScreenPoint = ScreenHeight * Math::Sqrt(3);

	if (ScreenNear <= v1.z && v1.z <= ScreenFar) {
		if (depthBuffer[(int)v1.y][(int)v1.x] > v1.z)
			depthBuffer[(int)v1.y][(int)v1.x] = v1.z;
		else
			return;
	}
	else {
		return;
	}

	SDL_FPoint temp = {};
	temp.x = ScreenWidth / 2 + v1.x * nearScreenPoint / v1.z;
	temp.y = ScreenHeight / 2 - v1.y * nearScreenPoint / v1.z;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(renderer, (int)v1.x, (int)v1.y);
}
/**/void RenderEngine::drawLineOnCamera(Vector3 v1, Vector3 v2, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_FPoint fp1 = vector3ToFPoint(v1);
	SDL_FPoint fp2 = vector3ToFPoint(v2);
	SDL_RenderDrawLine(renderer, (int)fp1.x, (int)fp1.y, (int)fp2.x, (int)fp2.y);
	/*Vector3 v21 = basisConversionWithCamera(v1);
	Vector3 v22 = basisConversionWithCamera(v2);
	if (Math::Abs(v21.x - v22.x) > Math::Abs(v21.y - v22.y)) {
		// x가 더 긴 상황
	}
	else {
		//
	}*/
}
void RenderEngine::drawTriangleOnCamera(Vector3 v1, Vector3 v2, Vector3 v3, SDL_Color color) {
	SDL_Vertex vertices[3] = {
		{ vector3ToFPoint(v1), color },
		{ vector3ToFPoint(v2), color },
		{ vector3ToFPoint(v3), color },
	};
	const int indices[3] = { 0, 1, 2 };
	SDL_RenderGeometry(renderer, NULL, vertices, 3, indices, 3);
	/*Vector3 v21 = basisConversionWithCamera(v1);
	Vector3 v22 = basisConversionWithCamera(v2);
	Vector3 v23 = basisConversionWithCamera(v3);*/
}
#pragma endregion

void RenderEngine::moveTo(Vector3 location) {
	camera.moveTo(location);
}

void RenderEngine::lookAt(Vector3 target) {
	camera.lookAt(target);
}
