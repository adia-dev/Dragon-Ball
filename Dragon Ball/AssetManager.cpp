#include "AssetManager.h"

AssetManager* AssetManager::_instance = nullptr;

AssetManager::AssetManager()
{
	assert(_instance == nullptr);
	_instance = this;
}


sf::Texture& AssetManager::GetTexture(const std::string& fileName, PATHS path)
{
	
	auto filePath = Utils::getFilePath(fileName, path);

	auto& texMap = _instance->_textures;
	auto t = texMap.find(fileName);
	if (t != texMap.end())
		return t->second;
	else {
		auto& texture = texMap[fileName];
		texture.loadFromFile(filePath);
		return texture;
	}
}

sf::Font& AssetManager::GetFont(const std::string& fileName, PATHS path)
{
	auto filePath = Utils::getFilePath(fileName, path);


	auto& fontMap = _instance->_fonts;
	auto f = fontMap.find(fileName);
	if (f != fontMap.end())
		return f->second;
	else {
		auto& texture = fontMap[fileName];
		texture.loadFromFile(filePath);
		return texture;
	}
}
