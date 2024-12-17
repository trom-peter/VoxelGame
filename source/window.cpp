#include "window.h"
#define SDL_MAIN_HANDLED
//#define SHOW_FPS

Window::Window(int width, int height, const char* title) : w(width), h(height), active(true), window(nullptr), glContext(nullptr) {}

bool Window::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    setupOpenGLAttributes();

    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;
    window = SDL_CreateWindow("MINECRAFT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
    if (!window) {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // Activate V-Sync

    SDL_SetRelativeMouseMode(SDL_TRUE);

    setupTimeCount();

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // 4x Multisampling

    return true;
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(window);
}

void Window::quit() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::setupOpenGLAttributes() {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    #ifdef _DEBUG
    std::cout << "DEBUG MODE ENABLED" << std::endl;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    #endif
}

void Window::setupTimeCount() {
    perfCounterFrequency = SDL_GetPerformanceFrequency();
    lastCounter = SDL_GetPerformanceCounter();
    delta = 0.0f;
    time = 0.0f;
    FPS = 0.0f;
}

void Window::increaseTime() {
    time += delta;
}

void Window::updateTime() {
    uint64_t endCounter = SDL_GetPerformanceCounter();
    uint64_t counterElapsed = SDL_GetPerformanceCounter() - lastCounter;
    delta = (float)counterElapsed / (float)perfCounterFrequency;
    FPS = (int)((float)perfCounterFrequency / (float)counterElapsed);
#ifdef SHOW_FPS
    std::cout << FPS << std::endl;
#endif
    lastCounter = endCounter;
}

void Window::handleInputs(InputHandler& ih) {
    if (ih.isKeyPressed(SDL_SCANCODE_ESCAPE) && active) setActive(false);
    if (ih.isButtonPressed(SDL_BUTTON_LEFT) && !active) setActive(true);
}

void Window::setActive(bool active) {
    SDL_SetRelativeMouseMode(active ? SDL_TRUE : SDL_FALSE);
    this->active = active;
}

float Window::getWidth() {
    return w;
}

float Window::getHeight() {
    return h;
}

float Window::getDelta() {
    return delta;
}

bool Window::isActive() {
    return active;
}