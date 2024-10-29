#include "GameManager.h"

GameManager::GameManager(uint32_t width, uint32_t height, const std::string& title, bool fullscreen)
{
	if (fullscreen)
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

	controlCharacter(0);

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
				_controlledCharacter->toggleUI();
				break;
			case sf::Keyboard::Space:
				_controlledCharacter->toggleFly();
				break;
			case sf::Keyboard::Num0:
				controlCharacter(0);
				break;
			case sf::Keyboard::Num1:
				controlCharacter(1);
				break;
			case sf::Keyboard::T:
				_keyMap["Teleport"] = true;
				break;
			case sf::Keyboard::LAlt:
				_keyMap["Transform"] = true;
				break;
			case sf::Keyboard::P:
				_chargeAttack = true;
				break;
			case sf::Keyboard::O:
				_chargeFire = true;
				_keyMap["Fire"] = true;
				break;
			}
		}
	}

	if (event.type == sf::Event::KeyReleased)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::P:
			if (_chargeAttack && _controlledCharacter != nullptr)
			{
				_chargeAttack = false;
				_keyMap["Attack"] = true;
				_controlledCharacter->attack(_chargeAttackTimer);
				_chargeAttackTimer = 1.f;

			}
			break;
		case sf::Keyboard::O:
			if (_chargeFire && _controlledCharacter != nullptr)
			{
				_chargeFire = false;
				_controlledCharacter->fire(_chargeFireTimer);
				_chargeFireTimer = 1.f;

			}
			break;
		}

	}

	if (event.type == sf::Event::JoystickButtonPressed)
	{
		switch (event.joystickButton.button)
		{
		case 0:
			_controlledCharacter->toggleFly();
			break;
		case 1:
			_chargeFire = true;
			_keyMap["Fire"] = true;
			break;
		case 2:
			_chargeAttack = true;
			break;
		case 3:
			_keyMap["Teleport"] = true;
			break;
		}
	}

	if (event.type == sf::Event::JoystickButtonReleased)
	{
		switch (event.joystickButton.button)
		{

		case 1:
			if (_chargeFire && _controlledCharacter != nullptr)
			{
				_chargeFire = false;
				_controlledCharacter->fire(_chargeFireTimer);
				_chargeFireTimer = 1.f;

			}
			break;
		case 2:
			if (_chargeAttack && _controlledCharacter != nullptr)
			{
				_chargeAttack = false;
				_keyMap["Attack"] = true;
				_controlledCharacter->attack(_chargeAttackTimer);
				_chargeAttackTimer = 1.f;

			}
			break;
		case 9:
			_keyMap["Transform"] = true;
			break;
		}
	}
}


void GameManager::checkInputs()
{

	if (_chargeAttack) {
		_chargeAttackTimer += _deltaTime.asSeconds() * 10.f;
	}

	if (_chargeFire) {
		_chargeFireTimer += _deltaTime.asSeconds() * 3.f;
	}

	float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
	float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

	_keyMap["Up"] = keyPressed(sf::Keyboard::W) || y <= -20.f;
	_keyMap["Left"] = keyPressed(sf::Keyboard::A) || x <= -20.f;
	_keyMap["Down"] = keyPressed(sf::Keyboard::S) || y >= 20.f;
	_keyMap["Right"] = keyPressed(sf::Keyboard::D) || x >= 20.f;

	_keyMap["Jump"] = keyPressed(sf::Keyboard::Space) || sf::Joystick::isButtonPressed(0, 0);
	_keyMap["Teleport"] = keyPressed(sf::Keyboard::T);
	_keyMap["Attack"] = keyPressed(sf::Keyboard::P);

	_keyMap["Special_1"] = keyPressed(sf::Keyboard::LShift) || sf::Joystick::isButtonPressed(0, 4);
	_keyMap["Special_2"] = keyPressed(sf::Keyboard::LControl) || sf::Joystick::isButtonPressed(0, 5);

	_keyMap["Taunt"] = keyPressed(sf::Keyboard::I) || sf::Joystick::isButtonPressed(0, 6);
	_keyMap["Charge"] = keyPressed(sf::Keyboard::M) || sf::Joystick::getAxisPosition(0, sf::Joystick::Z) >= 20.f;


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
	//ui += "\nLoad Attack : " + std::to_string(_chargeAttackTimer);
	_uiText.setString(ui);

	_updateCnt++;
}

void GameManager::render()
{
	_window->clear();

	_window->draw(_ground);



	for (auto& character : _characters) {
		if(character != _controlledCharacter)
			character->render(_window);
	}

	_controlledCharacter->render(_window);


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
