#pragma once
#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL.h>
#include "input_handler.h"

class InputHandler;

class Window {
public:
    Window(int width, int height, const char* title);

    bool init();

    void swapBuffers();

    void quit();

    void updateTime();

    int getWidth();

    int getHeight();

    float getDelta();

    void setActive(bool active);

    bool isActive();

    void handleInputs(InputHandler* ih);

private:
    void setupOpenGLAttributes();
    void setupTimeCount();

    const char* title;
    int width, height;
    bool active; //is this window being used
    SDL_Window* window;
    SDL_GLContext glContext;
    uint64_t perfCounterFrequency;
    uint64_t lastCounter;
    float time;
    float delta;
    int FPS;
};