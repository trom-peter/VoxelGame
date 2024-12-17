#pragma once
#include "window.h"

class InputHandler {
public:
	InputHandler();

	void handleEvents(bool& running);
	bool isKeyPressed(SDL_Scancode key);
	bool isButtonPressed(int button);
	int getRelativeX();
	int getRelativeY();
	Uint8 getCurrentKeyStates();

private:
	const Uint8* currentKeyStates;
	Uint32 relativeMouseState;
	int relX, relY;
};