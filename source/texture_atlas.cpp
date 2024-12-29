#include "texture_atlas.h"

TextureAtlas::TextureAtlas(std::string filename, unsigned int atlasWidth, unsigned int atlasHeight, unsigned int tileWidth, unsigned int tileHeight) : 
	filename(filename), atlasWidth(atlasWidth), atlasHeight(atlasHeight), tileWidth(tileWidth), tileHeight(tileHeight)
{
	TextureLoader::loadTexture(filename, &id);
}