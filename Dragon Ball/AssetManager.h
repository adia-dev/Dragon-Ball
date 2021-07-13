#pragma once

#include "Utils.h"

class AssetManager
{
public:
	AssetManager();

	static sf::Texture& GetTexture(const std::string& fileName, PATHS path = PATHS::NONE);
	static sf::Font& GetFont(const std::string& fileName, PATHS path = PATHS::NONE);

private:

	static AssetManager* _instance;

	std::map<std::string, sf::Texture> _textures;
	std::map<std::string, sf::Font> _fonts;

};

