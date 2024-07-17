#pragma once
#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <fstream>
#include <vector>
#include "raylib.h"
#include "Cube.h"

using namespace std;

constexpr int screenWidth = 1920;
constexpr int screenHeight = 1080;

class RenderEngine {
private:
	Camera camera;
	int cameraMode;

	string fileName;
	vector<Cube> cubes;

	void initWindow() {
		InitWindow(screenWidth, screenHeight, "Menger sponge 81x81x81");
		DisableCursor();
		SetTargetFPS(60);
	}
	void initCamera() {
		// Define the camera to look into our 3d world (position, target, up vector)
		camera.position = { 0.0f, 2.0f, 10.0f };
		camera.target = { 0.0f, 1.0f, 0.0f };
		camera.up = { 0.0f, 1.0f, 0.0f };
		camera.fovy = 60.0f;
		camera.projection = CAMERA_PERSPECTIVE;

		//int cameraMode = CAMERA_FIRST_PERSON;
		//cameraMode = CAMERA_THIRD_PERSON;
		cameraMode = CAMERA_FREE;
	}
	void initCubes() {
		ifstream fin(fileName);
		int xLen, yLen, zLen;
		fin >> xLen >> yLen >> zLen;
		for (int x = 0; x < xLen; ++x) {
			for (int y = 0; y < yLen; ++y) {
				for (int z = 0; z < zLen; ++z) {
					static bool exist;
					fin >> exist;
					if (!exist)
						continue;
					cubes.emplace_back(Cube(
						{ (float)x, (float)y, (float)z },
						{
							(unsigned char)(255.0f * x / xLen),
							(unsigned char)(255.0f * y / yLen),
							(unsigned char)(255.0f * z / zLen), 255
						}));
				}
			}
		}
	}

	void cameraMove(Vector3 movement) {
		//camera.target.x += movement.x;
		//camera.target.y += movement.y;
		//camera.target.z += movement.z;
		//
		//camera.position.x += movement.x;
		//camera.position.y += movement.y;
		//camera.position.z += movement.z;
	}

	void updateCamera() {
		/* Switch camera mode */
		//if (IsKeyPressed(KEY_ONE)) {
		//	cameraMode = CAMERA_FREE;
		//	camera.up = { 0.0f, 1.0f, 0.0f }; // Reset roll
		//}
		//if (IsKeyPressed(KEY_TWO)) {
		//	cameraMode = CAMERA_FIRST_PERSON;
		//	camera.up = { 0.0f, 1.0f, 0.0f }; // Reset roll
		//}
		//if (IsKeyPressed(KEY_THREE)) {
		//	cameraMode = CAMERA_THIRD_PERSON;
		//	camera.up = { 0.0f, 1.0f, 0.0f }; // Reset roll
		//}
		//if (IsKeyPressed(KEY_FOUR)) {
		//	cameraMode = CAMERA_ORBITAL;
		//	camera.up = { 0.0f, 1.0f, 0.0f }; // Reset roll
		//}

		/* Switch camera projection */
		//if (IsKeyPressed(KEY_P)) {
		//	if (camera.projection == CAMERA_PERSPECTIVE) {
		//		// Create isometric view
		//		cameraMode = CAMERA_THIRD_PERSON;
		//		camera.position = { 0.0f, 2.0f, -100.0f };
		//		camera.target = { 0.0f, 2.0f, 0.0f };
		//		camera.up = { 0.0f, 1.0f, 0.0f };
		//		camera.projection = CAMERA_ORTHOGRAPHIC;
		//		camera.fovy = 20.0f;
		//	}
		//	else if (camera.projection == CAMERA_ORTHOGRAPHIC) {
		//		// Reset to default view
		//		cameraMode = CAMERA_THIRD_PERSON;
		//		camera.position = { 0.0f, 2.0f, 10.0f };
		//		camera.target = { 0.0f, 2.0f, 0.0f };
		//		camera.up = { 0.0f, 1.0f, 0.0f };
		//		camera.projection = CAMERA_PERSPECTIVE;
		//		camera.fovy = 60.0f;
		//	}
		//}

		if (IsKeyPressed(KEY_W))
			cameraMove({ 0, 0, +20 });
		if (IsKeyPressed(KEY_A))
			cameraMove({ -20, 0, 0 });
		if (IsKeyPressed(KEY_S))
			cameraMove({ 0, 0, -20 });
		if (IsKeyPressed(KEY_D))
			cameraMove({ +20, 0, 0 });
		if (IsKeyPressed(KEY_SPACE)) {
			if (IsKeyPressed(KEY_LEFT_SHIFT))
				cameraMove({ 0, -20, 0 });
			else
				cameraMove({ 0, +20, 0 });
		}

		UpdateCamera(&camera, cameraMode);
	}
	void drawBegin() {
		BeginDrawing();
		//ClearBackground(RAYWHITE);
		//ClearBackground(GREEN);
		ClearBackground(DARKGRAY);
	}
	void drawBegin3D() {
		BeginMode3D(camera);
	}
	void drawCubes() {
		for (int i = 0; i < cubes.size(); ++i) {
			const Cube& cube = cubes[i];
			DrawCube(
				cube.location, cube.size, cube.size, cube.size, cube.color);
			//DrawCubeWires(
			//	cube.location, cube.size, cube.size, cube.size, MAROON);
		}
		//	DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
		//	DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);

		// Draw player cube
		if (cameraMode == CAMERA_THIRD_PERSON) {
			DrawCube(camera.target,
				0.5f, 0.5f, 0.5f, PURPLE);
			DrawCubeWires(camera.target,
				0.5f, 0.5f, 0.5f, DARKPURPLE);
		}
	}
	void drawEnd3D() {
		EndMode3D();
	}
	void drawUI() {
		// Draw info boxes
		DrawRectangle(
			5, 5, 330, 75, Fade(SKYBLUE, 0.5f));
		DrawRectangleLines(
			5, 5, 330, 75, BLUE);

		DrawText("Camera controls:", 15, 15, 10, BLACK);
		DrawText("- Move keys: W, A, S, D", 15, 30, 10, BLACK);
		DrawText("                  Ctrl, Space", 15, 45, 10, BLACK);
		DrawText("- Look around: arrow keys or mouse", 15, 60, 10, BLACK);

		DrawRectangle(
			1720, 5, 195, 75, Fade(SKYBLUE, 0.5f));
		DrawRectangleLines(
			1720, 5, 195, 75, BLUE);

		DrawText("Camera status:",
			1730, 15, 10, BLACK);
		DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z),
			1730, 30, 10, BLACK);
		DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z),
			1730, 45, 10, BLACK);
		DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z),
			1730, 60, 10, BLACK);
	}
	void drawEnd() {
		EndDrawing();
	}

public:
	RenderEngine() {
		camera = {};
		cameraMode = NULL;
		fileName = "";
		cubes = vector<Cube>();
	}

	void init(string fileName) {
		this->fileName = fileName;
		initWindow();
		initCamera();
		initCubes();
	}
	void update() {
		updateCamera();
		
		drawBegin();
		drawBegin3D();
		drawCubes();

		drawEnd3D();
		drawUI();
		drawEnd();
	}
	bool isRunning() {
		return !WindowShouldClose();
	}
	void release() {
		CloseWindow();
	}
};

#endif // !RENDER_ENGINE_H