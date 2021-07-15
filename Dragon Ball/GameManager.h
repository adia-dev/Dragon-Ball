#pragma once

#include "Vegito.h"

struct KeyActions {
	std::string name;
	sf::Sprite s;

	KeyActions()
	{
		s.setTexture(AssetManager::GetTexture("kefla.png", PATHS::CHARACTERS));
		s.setTextureRect(sf::IntRect(68, 408, 33, 55));
	}

	KeyActions(const std::string& name)
		:name(name)
	{
		s.setTexture(AssetManager::GetTexture("kefla.png", PATHS::CHARACTERS));
		s.setTextureRect(sf::IntRect(68, 408, 33, 55));
	}

	float timer = 0.f;
};

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

	void controlCharacter(int index) {
		if (index >= 0 && index < _characters.size()) {
			for (auto& character : _characters)
				character->releaseControl();
			_controlledCharacter = _characters[index];
			_controlledCharacter->takeControl();
		}
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
	bool _chargeAttack = false;
	bool _chargeFire = false;
	float _chargeAttackTimer = 1.f;
	float _chargeFireTimer = 1.f;


	// Instances
	Utils _utils;
	AssetManager _manager;

	Vegito _vegito;
	Character _test;
	std::vector<Character*> _characters;
	Character* _controlledCharacter = nullptr;
};

