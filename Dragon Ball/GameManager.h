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

	void init();

	inline void updateDeltaTime();
	void handleEvents();
	void checkInputs();
	void update();
	void render();

	void quit();

	inline bool keyPressed(sf::Keyboard::Key key)
	{
		return sf::Keyboard::isKeyPressed(key);
	}

	// Variables
	sf::RenderWindow* _window = nullptr;
	sf::RectangleShape _ground;
	sf::Clock _clock;
	sf::Time _deltaTime;
	sf::Text _uiText;

	std::map<std::string, bool> _keyMap;

	bool _isPlaying = true;
	int _updateCnt = 0;
	int _fps = 0;


	// Instances
	Utils _utils;
	AssetManager _manager;

	Vegito _vegito;
	Character _test;
	std::vector<Character*> _characters;
};

