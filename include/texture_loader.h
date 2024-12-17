#pragma once
#include <string>
#include "renderer.h"

class TextureLoader {
public:
	TextureLoader();
	static void loadTexture(std::string filename, GLuint* id);

private:
};