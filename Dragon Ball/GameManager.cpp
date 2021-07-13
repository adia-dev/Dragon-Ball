#include "GameManager.h"

GameManager::GameManager(uint32_t width, uint32_t height, const std::string& title, bool fullscreen)
{
	if(fullscreen)
	_window = new sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Fullscreen);
	else
	_window = new sf::RenderWindow(sf::VideoMode(width, height), title);

	Utils::setWindowSize(_window->getSize());

	_uiText.setFont(AssetManager::GetFont("assets/fonts/Poppins/Poppins-Regular.ttf"));
	_uiText.setCharacterSize(16);
}

GameManager::GameManager(bool fullscreen)
{
	if (fullscreen)
		_window = new sf::RenderWindow(sf::VideoMode(W, H), "Game", sf::Style::Fullscreen);
	else
		_window = new sf::RenderWindow(sf::VideoMode(W, H), "Game");

	Utils::setWindowSize(_window->getSize());

	_uiText.setFont(AssetManager::GetFont("assets/fonts/Poppins/Poppins-Regular.ttf"));
	_uiText.setCharacterSize(16);
}

GameManager::~GameManager()
{
	delete(_window);
}

void GameManager::play()
{
	if (_window == nullptr) return;

	while (_isPlaying)
	{
		updateDeltaTime();
		handleEvents();
		update();
		render();
	}
}

inline void GameManager::updateDeltaTime()
{
	_deltaTime = _clock.restart();
}

void GameManager::handleEvents()
{
	sf::Event event;
	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			quit();

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				quit();
				break;
			}
		}
	}
}

void GameManager::update()
{

	_vegito.update(_deltaTime.asSeconds());

	// UI
	if (_updateCnt % 500 == 0) {
		_fps = 1 / _deltaTime.asSeconds();
		_updateCnt = 0;
	}

	std::string ui = "FPS : " + std::to_string(_fps);
	_uiText.setString(ui);

	_updateCnt++;
}

void GameManager::render()
{
	_window->clear();

	_vegito.render(_window);

	// Render of the UI
	_window->draw(_uiText);

	_window->display();
}

void GameManager::quit()
{
	_isPlaying = false;
	_window->close();
	std::cout << "~Bye" << std::endl;
}
