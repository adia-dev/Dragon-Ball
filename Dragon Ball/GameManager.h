#pragma once

#include "Vegito.h"

class GameManager
{

public:

	GameManager(uint32_t width, uint32_t height, const std::string& title, bool fullscreen = false);
	GameManager(bool fullscreen = false);

	~GameManager();

	void play();

private:

	inline void updateDeltaTime();
	void handleEvents();
	void update();
	void render();

	void quit();

	// Variables
	sf::RenderWindow* _window = nullptr;
	sf::Clock _clock;
	sf::Time _deltaTime;
	sf::Text _uiText;

	bool _isPlaying = true;
	int _updateCnt = 0;
	int _fps = 0;


	// Instances
	Utils _utils;
	AssetManager _manager;

	Vegito _vegito;
};

