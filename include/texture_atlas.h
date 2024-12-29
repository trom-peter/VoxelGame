#pragma once

#include <string>
#include "renderer.h"
#include "texture_loader.h"

struct TextureAtlas {
	TextureAtlas(std::string filename, unsigned int atlasWidth, unsigned int atlasHeight, unsigned int tileWidth, unsigned int tileHeight);

	GLuint id;
	std::string filename;
	unsigned int atlasWidth;
	unsigned int atlasHeight;
	unsigned int tileWidth;
	unsigned int tileHeight;
};