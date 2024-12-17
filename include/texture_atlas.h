#pragma once

#include <string>
#include "renderer.h"

struct TextureAtlas {
	TextureAtlas(std::string filename, unsigned int atlasWidth, unsigned int atlasHeight, unsigned int tileWidth, unsigned int tileHeight) :
		filename(filename), atlasWidth(atlasWidth), atlasHeight(atlasHeight), tileWidth(tileWidth), tileHeight(tileHeight) {}

	GLuint id;
	std::string filename;
	unsigned int atlasWidth;
	unsigned int atlasHeight;
	unsigned int tileWidth;
	unsigned int tileHeight;
};