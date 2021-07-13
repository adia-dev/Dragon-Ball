#include "Vegito.h"

Vegito::Vegito()
{
	_position = sf::Vector2f(400, 300);

	toggleUI();

	_vegitoUI.setFont(AssetManager::GetFont("Poppins/Poppins-Regular.ttf", PATHS::FONTS));
	_vegitoUI.move(0, 16.f);
	_vegitoUI.setCharacterSize(16);


}

void Vegito::init()
{
	_s.setTexture(AssetManager::GetTexture("vegito_blue.png", PATHS::CHARACTERS));

	_animator->loadAnimation("vegito_blue.xml", PATHS::ANIMATIONS);
	_animator->playAnimation("Idle_1", true);

	std::cout << "Vegito Blue" << std::endl;
	_name = "Vegito Blue";

	_position = sf::Vector2f(200, 300);
}


void Vegito::update(const float& deltaTime, std::map<std::string, bool>& keyMap)
{
	_canMove = true;
	_attack = false;

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
	updatePosition(deltaTime, keyMap);
	updateCharacterUI();
	updateAnimation(deltaTime);

	std::string vegitoUIStr = "Current Frame : " + std::to_string(_animator->getCurrentFrame());
	vegitoUIStr += "\nVelocity(" + std::to_string(_velocity.x) + ", " + std::to_string(_velocity.y) + ")\n";

	_vegitoUI.setString(vegitoUIStr);


}

void Vegito::updateState(std::map<std::string, bool>& keyMap)
{
	if (_controllable)
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

		if (keyMap["Special_1"] && keyMap["Special_2"])
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

	if (_position.x > 800)
		_position.x = 0;

	if (_position.x < 0)
		_position.x = 800;

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
				_animator->playAnimation("Idle_Powered", true);
			else
				_animator->playAnimation("Idle_2", true);
		}
		else {
			_animator->playAnimation("Idle_Fly", true);
		}
		break;
	case AnimationStates::GROUND_LEFT:
		_animator->playAnimation("Left_Ground");
		break;
	case AnimationStates::GROUND_RIGHT:
		_animator->playAnimation("Right_Ground");
		break;
	case AnimationStates::AIR_LEFT:
		_animator->playAnimation("Left_Air");
		break;
	case AnimationStates::AIR_RIGHT:
		_animator->playAnimation("Right_Air");
		break;
	case AnimationStates::UP:
		_animator->playAnimation("Up");
		break;
	case AnimationStates::DOWN:
		_animator->playAnimation("Down");
		break;
	case AnimationStates::SLIDE:
		_animator->playAnimation("Left_Slide");
		break;
	case AnimationStates::TELEPORT:
		_animator->playAnimation("Teleport", false, true);
		break;
	case AnimationStates::ATTACK:
		_animator->playAnimation("Attack_" + std::to_string(_currentComboIndex), false, true);
		_currentComboIndex = (_currentComboIndex + 1) % 5 + 1;

		_attackCollider.setSize(sf::Vector2f(50.f, 80.f));
		_attack = true;
		if (_isLookingLeft)
			_attackCollider.setPosition(_position.x - _s.getGlobalBounds().width, _position.y - _s.getGlobalBounds().height * 1.25f);
		else
			_attackCollider.setPosition(_position.x + _s.getGlobalBounds().width / 2.f, _position.y - _s.getGlobalBounds().height * 1.25f);

		if (_target != nullptr)
		{
			if (_attackCollider.getGlobalBounds().contains(_target->getPosition()))
			{
				std::cout << "Attack from : " + _name + " to : " + _target->getName() << std::endl;
				int randomHitMultiplier = rand() % 5 + 1;
				if (_isLookingLeft) {
					_target->addForce(-1000.f * randomHitMultiplier, -1000.f * randomHitMultiplier);
				}
				else {
					_target->addForce(1000.f * randomHitMultiplier, -1000.f * randomHitMultiplier);
				}
				_target->playAnimation("Hit_Face_" + std::to_string(rand() % 2 + 1), false, true);

			}
		}

		break;
	case AnimationStates::SPECIAL_1:
		_animator->playAnimation("Special_1", false, true);
		break;
	case AnimationStates::SPECIAL_2:
		_animator->playAnimation("Special_2", false, true);
		break;
	case AnimationStates::TRANSFORM:
		_animator->playAnimation("Transform_1", false, true);
		_currentAnimationState = AnimationStates::IDLE;
		break;
	case AnimationStates::HIT:
		_animator->playAnimation("Hit_Chest", false, true);
		break;
	case AnimationStates::GUARD:
		_animator->playAnimation("Guard");
		break;
	case AnimationStates::CHARGE:
		_animator->playAnimation("Charge", true);
		chargeKI(20.f * deltaTime);
		break;
	case AnimationStates::TAUNT:
		_animator->playAnimation("Taunt", false, true);
		break;
	}

	_animator->update(deltaTime);

}



void Vegito::render(sf::RenderTarget* target)
{
	if (target == nullptr) return;

	target->draw(_s);
	if (_renderUI) {
		target->draw(_vegitoUI);
		target->draw(_originUI);
		target->draw(_groundCastUI);
		target->draw(_hpUI);
		target->draw(_kiUI);
	}

	if (_attack)
		target->draw(_attackCollider);
}



