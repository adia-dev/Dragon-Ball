#include "Vegito.h"

Vegito::Vegito()
{
	_position = sf::Vector2f(400, 300);

	//toggleUI();

	_vegitoUI.setFont(AssetManager::GetFont("Poppins/Poppins-Regular.ttf", PATHS::FONTS));
	_vegitoUI.move(0, 150.f);
	_vegitoUI.setCharacterSize(16);


}

void Vegito::init()
{
	_s.setTexture(AssetManager::GetTexture("kefla.png", PATHS::CHARACTERS));

	_animator->loadAnimation("kefla.xml", PATHS::ANIMATIONS);
	_animator->playAnimation(std::to_string(_currentTransformation) + "_Idle", true);

	_name = "Kefla";
	std::cout << _name << std::endl;

	_position = sf::Vector2f(200, 300);

	_characterCollider.setSize(sf::Vector2f(_s.getGlobalBounds().width, _s.getGlobalBounds().height));

	_headshot.setTexture(AssetManager::GetTexture("kefla.png", PATHS::CHARACTERS));
	_headshot.setTextureRect(_animator->getAnimationFrame("Headshot"));

	_hpUI.setFillColor(sf::Color::Green);
	_hpUI.setSize(sf::Vector2f(100.f, 15.f));
	_hpUI.setPosition(50.f, 50.f);

	_kiUI.setFillColor(sf::Color::Blue);
	_kiUI.setSize(sf::Vector2f(100.f, 8.f));
	_kiUI.setPosition(_kiUI.getSize().x - 50.f, 75.f);

	_headshot.setPosition(5.f, 40.f);

}


void Vegito::update(const float& deltaTime, std::map<std::string, bool>& keyMap)
{
	_canMove = true;
	_attack = false;
	_isDead = _hp <= 0.f;



	_colliderDebugTimer += deltaTime;

	if (_ki <= 0) {
		_rushing = false;
	}

	if (Utils::distance(_position, _target->getPosition()) <= 35.f && _rushing) {
		_rushing = false;
		_acceleration = sf::Vector2f(0.f, 0.f);
		_velocity = sf::Vector2f(0.f, 0.f);
	}

	_attackTimer += deltaTime;


	if (_target != nullptr)
		_isLookingLeft = _position.x > _target->getPosition().x;
	else
		_isLookingLeft = _position.x > 400;

	_animator->flip(_isLookingLeft);

	if (_isFlying)
	{
		useKi(2.f * deltaTime);
		if (_ki <= 25.f)
			_isFlying = false;
	}

	updateState(keyMap);
	updateCharacterUI();
	updateAnimation(deltaTime);
	updatePosition(deltaTime, keyMap);
	updateProjectile(deltaTime);

	std::string vegitoUIStr = "Current Frame : " + std::to_string(_animator->getCurrentFrame());
	vegitoUIStr += "\nVelocity(" + std::to_string(_velocity.x) + ", " + std::to_string(_velocity.y) + ")\n";
	vegitoUIStr += "State : " + _animationStatesNames[(int)_currentAnimationState];

	_vegitoUI.setString(vegitoUIStr);

	_characterCollider.setSize(sf::Vector2f(_s.getGlobalBounds().width, _s.getGlobalBounds().height));
	_characterCollider.setPosition(_position.x - _s.getGlobalBounds().width / 2.f, _position.y - _s.getGlobalBounds().height);


}

void Vegito::updateState(std::map<std::string, bool>& keyMap)
{
	if (_controllable && !_rushing)
	{
		if (_isGrounded)
		{
			if (keyMap["Left"])
			{
				_currentAnimationState = (_isLookingLeft ? AnimationStates::GROUND_RIGHT : AnimationStates::GROUND_LEFT);
				addForce(-_movingSpeed, 0.f);
			}
			else if (keyMap["Right"])
			{
				_currentAnimationState = (_isLookingLeft ? AnimationStates::GROUND_LEFT : AnimationStates::GROUND_RIGHT);
				addForce(_movingSpeed, 0.f);
			}
			else if (keyMap["Down"])
			{
				_canMove = false;
				_currentAnimationState = AnimationStates::GUARD;
			}
			else {
				_currentAnimationState = AnimationStates::IDLE;
			}

			if (keyMap["Jump"]) {
				_currentAnimationState = AnimationStates::UP;
				addForce(0, -1500.f);
				_rushing = false;
			}
		}
		else {
			if (_canMove) {
				if (keyMap["Left"])
				{
					_currentAnimationState = (_isLookingLeft ? AnimationStates::AIR_RIGHT : AnimationStates::AIR_LEFT);
					addForce(-_movingSpeed * 1.25f, 0.f);
				}
				else if (keyMap["Right"])
				{
					_currentAnimationState = (_isLookingLeft ? AnimationStates::AIR_LEFT : AnimationStates::AIR_RIGHT);
					addForce(_movingSpeed * 1.25f, 0.f);
				}
				else {
					_currentAnimationState = AnimationStates::IDLE;
				}

				if (keyMap["Up"])
				{
					if (!_isFlying)
					{
						_velocity.y = 0.f;
						_isFlying = true;
					}

					_currentAnimationState = AnimationStates::UP;
					addForce(0.f, -45.f);
				}
				else if (keyMap["Down"])
				{
					_currentAnimationState = AnimationStates::DOWN;
					addForce(0.f, 45.f);
				}
			}
		}

		if (keyMap["Left"])
		{
			_currentComboIndex = 3;
		}
		else if (keyMap["Right"])
		{
			_currentComboIndex = 2;
		}
		else if (keyMap["Up"])
		{
			_currentComboIndex = 4;
		}
		else if (keyMap["Down"])
		{
			_currentComboIndex = 5;
		}
		else {
			_currentComboIndex = 1;
		}

		if (keyMap["Special_1"] && keyMap["Special_2"] || keyMap["Transform"])
		{
			_currentAnimationState = AnimationStates::TRANSFORM;
			_ki = 85.f;
		}
		else if (keyMap["Special_1"] && _ki >= 50.f)
		{
			_currentAnimationState = AnimationStates::SPECIAL_1;
			useKi(50.f);
		}
		else if (keyMap["Special_2"] && _ki >= 85.f)
		{
			_currentAnimationState = AnimationStates::SPECIAL_2;
			useKi(70.f);
		}

		if (keyMap["Charge"])
		{
			_currentAnimationState = AnimationStates::CHARGE;
			_canMove = false;
		}

		if (keyMap["Taunt"])
		{
			_currentAnimationState = AnimationStates::TAUNT;
			_canMove = false;
		}

		if (keyMap["Attack"])
		{
			if (_attackTimer > _attackDelay) {
				_currentAnimationState = AnimationStates::ATTACK;
				_canMove = false;
				_attack = true;
				_attackTimer = 0.f;
			}
		}

		if (keyMap["Charge"] && keyMap["Jump"] && _target != nullptr && _ki >= 10.f) {
			_rushing = true;
			if (!_target->isGrounded())
				_isFlying = true;
			_currentAnimationState = AnimationStates::RUSHING;
		}

		if (keyMap["Teleport"] && _ki >= 15.f)
		{
			if (!_target->isGrounded())
				_isFlying = true;
			_currentAnimationState = AnimationStates::TELEPORT;
		}

		if (keyMap["Fire"])
		{
			_currentAnimationState = AnimationStates::FIRE;
			_canMove = false;
		}
	}
}

void Vegito::updatePosition(const float& deltaTime, std::map<std::string, bool>& keyMap)
{


	sf::Vector2f friction = -1.f * Utils::normalize(_velocity) * Utils::magnitude(_velocity) * COEF_FRICTION;

	if (!_isFlying) {
		addForce(0.f, GRAVITY * GRAVITY_MULTIPLIER * _mass);
		addForce(friction);
	}
	else {
		addForce(friction / 2.f);
	}

	_velocity += _acceleration * deltaTime;
	if (_canMove)
		_position += _velocity;

	_isGrounded = _position.y >= 475;

	if (_position.y >= 500)
	{
		_velocity.y = 0.f;
		_acceleration.y = 0.f;
		_position.y = 500;
		_isFlying = false;
	}

	if (_rushing && _target != nullptr)
	{
		useKi(30 * deltaTime);

		_position = Utils::vLerp(_position, _target->getPosition(), deltaTime * 5.f);

		//addForce(Utils::normalize(targetPos) * 250.f);
	}

	if (_position.x > 800) {
		_position.x = 0;
		//_velocity.x = -_velocity.x;
	}

	if (_position.x < 0) {
		_position.x = 800;
		//_velocity.x = -_velocity.x;
	}


	if (_isLookingLeft)
		_s.setPosition(_position.x - _s.getGlobalBounds().width, _position.y);
	else
		_s.setPosition(_position);
	_acceleration = sf::Vector2f(0, 0);

}

void Vegito::updateAnimation(const float& deltaTime)
{

	switch (_currentAnimationState)
	{
	case AnimationStates::IDLE:
		if (_isGrounded)
		{
			if (_ki >= 85.f)
				_animator->playAnimation(std::to_string(_currentTransformation) + "_Idle_Powered", false, true);
			else
				_animator->playAnimation(std::to_string(_currentTransformation) + "_Idle", true);
		}
		else {
			_animator->playAnimation(std::to_string(_currentTransformation) + "_Idle_Fly", true);
		}
		break;
	case AnimationStates::GROUND_LEFT:
		if(_currentTransformation == 0 || _currentTransformation == 2)
		_animator->flip(true);
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Left_Ground");
		break;
	case AnimationStates::GROUND_RIGHT:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Right_Ground");
		break;
	case AnimationStates::AIR_LEFT:
		if (_currentTransformation == 0 || _currentTransformation == 2)
			_animator->flip(true);
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Left_Air");
		break;
	case AnimationStates::AIR_RIGHT:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Right_Air");
		break;
	case AnimationStates::UP:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Up");
		break;
	case AnimationStates::DOWN:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Down");
		break;
	case AnimationStates::SLIDE:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Left_Slide");
		break;
	case AnimationStates::TELEPORT:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Teleport", false, true);
		_currentAnimationState = AnimationStates::IDLE;
		if (_isLookingLeft) {
			_position.x = _target->getPosition().x - 50.f;
			_position.y = _target->getPosition().y;
		}
		else {
			_position.x = _target->getPosition().x + 50.f;
			_position.y = _target->getPosition().y;
			
		}
		useKi(15.f);
		break;
	case AnimationStates::ATTACK:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Attack_" + std::to_string(_currentComboIndex), false, true);
		_attackCollider.setSize(sf::Vector2f(50.f, 80.f));
		_attack = false;
		break;
	case AnimationStates::SPECIAL_1:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Special_1", false, true);
		break;
	case AnimationStates::SPECIAL_2:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Special_2", false, true);
		break;
	case AnimationStates::TRANSFORM:
		_currentTransformation++;
		if (_currentTransformation >= _animator->getMaxTransformation())
			_currentTransformation = _animator->getMaxTransformation();
		_animator->playAnimation("Transform_" + std::to_string(_currentTransformation), false, true);
		_currentAnimationState = AnimationStates::IDLE;
		break;
	case AnimationStates::HIT:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Hit_Chest", false, true);
		break;
	case AnimationStates::GUARD:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Guard");
		break;
	case AnimationStates::CHARGE:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Charge", true);
		chargeKI(50.f * deltaTime);
		break;
	case AnimationStates::TAUNT:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Taunt", false, true);
		break;
	case AnimationStates::RUSHING:
		//_animator->playAnimation(std::to_string(_currentTransformation) + "_Right_Ground");
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Dash");
		break;
	case AnimationStates::KO:
		//_animator->playAnimation(std::to_string(_currentTransformation) + "_Right_Ground");
		_animator->playAnimation(std::to_string(_currentTransformation) + "_KO");
		break;
	case AnimationStates::FIRE:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Fire", false, true);
		break;
	}

	_animator->update(deltaTime);

}



void Vegito::render(sf::RenderTarget* target)
{
	if (target == nullptr) return;

	target->draw(_headshot);
	target->draw(_hpUI);
	target->draw(_kiUI);

	target->draw(_s);
	if (_renderUI) {
		target->draw(_vegitoUI);
		target->draw(_originUI);
		target->draw(_groundCastUI);
		target->draw(_characterCollider);
		if (_colliderDebugTimer <= 1.5f) {
			target->draw(_attackCollider);
		}

	}

	for (auto& projectile : _projectiles)
		target->draw(projectile);

}



