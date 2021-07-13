#include "GameManager.h"

GameManager::GameManager(uint32_t width, uint32_t height, const std::string& title, bool fullscreen)
{
	if(fullscreen)
	_window = new sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Fullscreen);
	else
	_window = new sf::RenderWindow(sf::VideoMode(width, height), title);

	init();
}

GameManager::GameManager(bool fullscreen)
{
	if (fullscreen)
		_window = new sf::RenderWindow(sf::VideoMode(W, H), "Game", sf::Style::Fullscreen);
	else
		_window = new sf::RenderWindow(sf::VideoMode(W, H), "Game");

	init();
}

GameManager::~GameManager()
{

	for (auto* character : _characters)
		delete(character);

	delete(_window);
}


void GameManager::init()
{

	_characters.push_back(new Vegito());
	_characters.push_back(new Character());

	for (auto& character : _characters)
		character->init();

	_characters[0]->takeControl();
	_characters[0]->setTarget(_characters[1]);
	_characters[1]->setTarget(_characters[0]);

	_ground.setPosition(0, 500);
	_ground.setSize(sf::Vector2f(800, 100));
	_ground.setFillColor(sf::Color(255, 255, 255, 100));

	Utils::setWindowSize(_window->getSize());

	_uiText.setFont(AssetManager::GetFont("assets/fonts/Poppins/Poppins-Regular.ttf"));
	_uiText.setCharacterSize(16);

}

void GameManager::play()
{
	if (_window == nullptr) return;

	while (_isPlaying)
	{
		_keyMap.clear();

		updateDeltaTime();
		handleEvents();
		checkInputs();
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
			case sf::Keyboard::Tab:
				_characters[0]->toggleUI();
				break;
			case sf::Keyboard::Space:
				_characters[0]->toggleFly();
				break;
			case sf::Keyboard::Num1:
				_characters[0]->releaseControl();
				_characters[1]->takeControl();
				break;
			case sf::Keyboard::Num0:
				_characters[1]->releaseControl();
				_characters[0]->takeControl();
				break;
			}

		}
	}
}

void GameManager::checkInputs()
{

	_keyMap["Up"] = keyPressed(sf::Keyboard::Z);
	_keyMap["Left"] = keyPressed(sf::Keyboard::Q);
	_keyMap["Down"] = keyPressed(sf::Keyboard::S);
	_keyMap["Right"] = keyPressed(sf::Keyboard::D);

	_keyMap["Jump"] = keyPressed(sf::Keyboard::Space);
	_keyMap["Attack"] = keyPressed(sf::Keyboard::P);

	_keyMap["Special_1"] = keyPressed(sf::Keyboard::LShift);
	_keyMap["Special_2"] = keyPressed(sf::Keyboard::LControl);

	_keyMap["Charge"] = keyPressed(sf::Keyboard::O);

	_keyMap["Taunt"] = keyPressed(sf::Keyboard::I);

}

void GameManager::update()
{

	for (auto& character : _characters)
		character->update(_deltaTime.asSeconds(), _keyMap);

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

	_window->draw(_ground);
	

	for (auto& character : _characters)
		character->render(_window);

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
