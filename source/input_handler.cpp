#include "input_handler.h"

InputHandler::InputHandler() : currentKeyStates(SDL_GetKeyboardState(NULL)), relativeMouseState(SDL_GetMouseState(NULL, NULL)) {}

void InputHandler::handleEvents(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
    relativeMouseState = SDL_GetRelativeMouseState(&relX, &relY);
    currentKeyStates = SDL_GetKeyboardState(NULL);
}

bool InputHandler::isKeyPressed(SDL_Scancode key) {
    return currentKeyStates[key] == 1;
}

bool InputHandler::isButtonPressed(int button) {
    return relativeMouseState & SDL_BUTTON(button);
}

int InputHandler::getRelativeX() {
    return relX;
}

int InputHandler::getRelativeY() {
    return relY;
}

Uint8 InputHandler::getCurrentKeyStates() {
    return *currentKeyStates;
}