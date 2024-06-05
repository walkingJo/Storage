#pragma once
#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H

#include <SDL.h>

class MouseCoord {
public:
	int xCoord;
	int yCoord;

	MouseCoord() : xCoord(0), yCoord(0) {}
	MouseCoord(int x, int y) : xCoord(x), yCoord(y) {}
};

enum class KeyboardState {
	COMMON,
	PRESSED,
	HOLDED,
	RELEASED,
};

class InputProcessor {
public:
	InputProcessor() {
		newCoord = { 0,0 };
		oldCoord = { 0,0 };
		isMousePressed = false;
		isMouseMoved = false;
		keyState = SDL_GetKeyboardState(NULL);

		processRunning = true;
	}
	void update() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				processRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				newCoord = MouseCoord(event.button.x, event.button.y);
				oldCoord = MouseCoord(event.button.x, event.button.y);
				isMousePressed = true;
				isMouseMoved = false;
				break;
			case SDL_MOUSEBUTTONUP:
				isMousePressed = false;
				isMouseMoved = false;
				break;
			case SDL_MOUSEMOTION:
				if (isMousePressed) {
					newCoord = MouseCoord(event.motion.x, event.motion.y);
				}
				isMouseMoved = true;
				break;
			}
		}

		keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_ESCAPE]) {
			processRunning = false;
		}
		for (int i = 4; i <= 290; ++i) {
			if (keyState[i]) {
				switch (keyStateEx[i]) {
				case KeyboardState::COMMON:
				case KeyboardState::RELEASED:	keyStateEx[i] = KeyboardState::PRESSED;		break;
				case KeyboardState::PRESSED:	keyStateEx[i] = KeyboardState::HOLDED;		break;
				}
			}
			else {
				switch (keyStateEx[i]) {
				case KeyboardState::PRESSED:
				case KeyboardState::HOLDED:		keyStateEx[i] = KeyboardState::RELEASED;	break;
				case KeyboardState::RELEASED:	keyStateEx[i] = KeyboardState::COMMON;		break;
				}
			}
		}
	}

	const MouseCoord& getMouseCoord() {
		return newCoord;
	}
	const MouseCoord& getMouseDeltaCoord() {
		return MouseCoord(
			newCoord.xCoord - oldCoord.xCoord,
			newCoord.yCoord - oldCoord.yCoord
		);
	}

	KeyboardState getKeyState(int keyCode) {
		return keyStateEx[keyCode];
	}
	bool isKeyPressed(int keyCode) {
		return keyState[keyCode];
	}
	bool isKeyReleased(int keyCode) {
		return !keyState[keyCode];
	}

	KeyboardState getUpKeyState() {
		return mixKeyboardState(
			keyStateEx[SDL_SCANCODE_W],
			keyStateEx[SDL_SCANCODE_UP]
		);
	}
	KeyboardState getDownKeyState() {
		return mixKeyboardState(
			keyStateEx[SDL_SCANCODE_S],
			keyStateEx[SDL_SCANCODE_DOWN]
		);
	}
	KeyboardState getRightKeyState() {
		return mixKeyboardState(
			keyStateEx[SDL_SCANCODE_D],
			keyStateEx[SDL_SCANCODE_RIGHT]
		);
	}
	KeyboardState getLeftKeyState() {
		return mixKeyboardState(
			keyStateEx[SDL_SCANCODE_A],
			keyStateEx[SDL_SCANCODE_LEFT]
		);
	}
	bool isUpKeyPressed() {
		return isKeyPressed(getUpKeyState());
	}
	bool isDownKeyPressed() {
		return isKeyPressed(getDownKeyState());
	}
	bool isRightKeyPressed() {
		return isKeyPressed(getRightKeyState());
	}
	bool isLeftKeyPressed() {
		return isKeyPressed(getLeftKeyState());
	}

	bool isRunning() {
		return processRunning;
	}

private:
	MouseCoord newCoord;
	MouseCoord oldCoord;
	const Uint8* keyState;
	KeyboardState keyStateEx[291] = {};
	bool isMousePressed; //언젠가는 rMouse, lMouse로 분리한다.
	bool isMouseMoved;

	bool processRunning;

	KeyboardState mixKeyboardState(KeyboardState state1, KeyboardState state2) {
		if (state1 == state2)
			return state1;

		if (state1 == KeyboardState::COMMON ||
			state1 == KeyboardState::RELEASED)
			return state2;
		if (state2 == KeyboardState::COMMON ||
			state2 == KeyboardState::RELEASED)
			return state1;

		if ((state1 == KeyboardState::PRESSED || state1 == KeyboardState::HOLDED) &&
			(state2 == KeyboardState::PRESSED || state2 == KeyboardState::HOLDED))
			return KeyboardState::HOLDED;

		return KeyboardState::COMMON;
	}
	bool isKeyPressed(KeyboardState state) {
		switch (state) {
		case KeyboardState::PRESSED:
		case KeyboardState::HOLDED:
			return true;
		case KeyboardState::COMMON:
		case KeyboardState::RELEASED:
		default:
			return false;
		}
	}

};

#endif // !INPUT_PROCESSOR_H