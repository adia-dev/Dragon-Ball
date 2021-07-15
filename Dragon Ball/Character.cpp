#include "Character.h"

Character::Character()
{
	_animator = new Animator(_s);


	_originUI.setFillColor(sf::Color::Red);
	_originUI.setRadius(3.f);
	_groundCastUI.setSize(sf::Vector2f(1.f, 50.f));



	_attackCollider.setFillColor(sf::Color::Transparent);
	_attackCollider.setOutlineThickness(1.f);
	_attackCollider.setOutlineColor(sf::Color::Red);

	_characterCollider.setFillColor(sf::Color::Transparent);
	_characterCollider.setOutlineThickness(1.f);
	_characterCollider.setOutlineColor(sf::Color::Yellow);

	_characterCollider.setSize(sf::Vector2f(_s.getGlobalBounds().width, _s.getGlobalBounds().height));

	_headshot.setPosition(800.f - 45.f, 40.f);

}

Character::~Character()
{
	delete(_animator);
}

void Character::init()
{

	_hpUI.setFillColor(sf::Color::Green);
	_hpUI.setSize(sf::Vector2f(100.f, 15.f));
	_hpUI.setPosition(800.0f - _hpUI.getSize().x - 50.f, 50.f);

	_kiUI.setFillColor(sf::Color::Blue);
	_kiUI.setSize(sf::Vector2f(100.f, 8.f));
	_kiUI.setPosition(800.0f - _kiUI.getSize().x - 50.f, 75.f);

	_s.setTexture(AssetManager::GetTexture("goku_ssjg.png", PATHS::CHARACTERS));
	_animator->loadAnimation("goku_ssjg.xml", PATHS::ANIMATIONS);
	_animator->playAnimation("Idle_1", true);

	_headshot.setTexture(AssetManager::GetTexture("goku_ssjg.png", PATHS::CHARACTERS));
	_headshot.setTextureRect(_animator->getAnimationFrame("Headshot"));


	std::cout << "Base character" << std::endl;

	_position = sf::Vector2f(600, 300);

}

void Character::update(const float& deltaTime, std::map<std::string, bool>& keyMap)
{
	if (_isDead)
	{
		_currentAnimationState = AnimationStates::KO;
		_isFlying = false;
		_rushing = false;
	}

	_canMove = true;
	_attack = false;
	_isDead = _hp <= 0.f;




	if (_target != nullptr && !_isDead)
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
	updateProjectile(deltaTime);

	_characterCollider.setSize(sf::Vector2f(_s.getGlobalBounds().width, _s.getGlobalBounds().height));
	_characterCollider.setPosition(_position.x - _s.getGlobalBounds().width / 2.f, _position.y - _s.getGlobalBounds().height);

}

void Character::render(sf::RenderTarget* target)
{
	if (target == nullptr) return;

	target->draw(_headshot);
	target->draw(_hpUI);
	target->draw(_kiUI);

	target->draw(_s);
	if (_renderUI) {
		target->draw(_originUI);
		target->draw(_groundCastUI);
		target->draw(_characterCollider);
		if (_attack)
			target->draw(_attackCollider);
	}



	for (auto& projectile : _projectiles)
		target->draw(projectile);

}

void Character::addForce(const sf::Vector2f& force)
{
	_acceleration += (force / _mass);
}

void Character::addForce(const float& x, const float& y)
{
	_acceleration += (sf::Vector2f(x, y) / _mass);
}

void Character::updateCharacterUI()
{
	_groundCastUI.setFillColor(_isGrounded ? sf::Color::Green : sf::Color::Red);
	if (_isLookingLeft) {
		_originUI.setPosition(_s.getPosition().x + _s.getGlobalBounds().width, _s.getPosition().y - 5.f);
		_groundCastUI.setPosition(_s.getPosition().x + _s.getGlobalBounds().width, _s.getPosition().y - _s.getGlobalBounds().height / 2.f);
	}
	else {
		_originUI.setPosition(_s.getPosition().x, _s.getPosition().y - 5.f);
		_groundCastUI.setPosition(_s.getPosition().x, _s.getPosition().y - _s.getOrigin().y / 2.f);
	}

	_hpUI.setSize(sf::Vector2f((_hp / 5000.f) * 100.f, 15.f));

	_kiUI.setSize(sf::Vector2f(_ki, 8.f));

}

void Character::playAnimation(const std::string& name, bool isLoop, bool isLocked)
{
	_animator->playAnimation(name, isLoop, isLocked);
}

void Character::takeDamage(float amount)
{
	_hp -= amount;
	if (_hp < 0.f)
		_hp = 0.f;
}

void Character::attack(float loadAttackFactor)
{
	//_currentComboIndex = (_currentComboIndex + 1) % 5 + 1;

	if (_isLookingLeft)
		_attackCollider.setPosition(_position.x - _s.getGlobalBounds().width, _position.y - _s.getGlobalBounds().height * 1.25f);
	else
		_attackCollider.setPosition(_position.x + _s.getGlobalBounds().width / 5.f, _position.y - _s.getGlobalBounds().height * 1.25f);

	if (_target != nullptr)
	{

		_colliderDebugTimer = 0.f;

		if (_target->getCharacterCollider().intersects(_attackCollider.getGlobalBounds()))
		{
			std::cout << "Attack from : " + _name + " to : " + _target->getName() << std::endl;

			int sideFactor = _isLookingLeft ? -1 : 1;

			switch (_currentComboIndex)
			{
			case 1:
				_target->addForce(sideFactor * 1000.f * loadAttackFactor, (_isFlying ? 100.f : -100.f) * loadAttackFactor);
				break;
			case 2:
				_target->addForce(sideFactor * 5000.f * loadAttackFactor, (_isFlying ? 10.f : -10.f) * loadAttackFactor);
				break;
			case 3:
				_target->addForce(sideFactor * 7000.f * loadAttackFactor, 100.f * loadAttackFactor);
				break;
			case 4:
				_target->addForce(sideFactor * 3000.f * loadAttackFactor, -7000.f * loadAttackFactor);
				_rushing = true;
				_isFlying = true;
				break;
			case 5:
				_target->addForce(sideFactor * 1000.f * loadAttackFactor, 7000.f * loadAttackFactor);
				_rushing = true;
				_isFlying = true;
				break;
			case 6:
				break;
			default:
				_target->addForce(sideFactor * 1000.f * loadAttackFactor, (_isFlying ? 100.f : -100.f) * loadAttackFactor);
				break;
			}

			_target->takeDamage((float)(rand() % 20 + 100) * loadAttackFactor);

			if (!_target->getIsDead())
				_target->playAnimation(std::to_string(_target->getCurrentTransformation()) + "_Hit_Face_" + std::to_string(rand() % 2 + 1), false, true);

			_target->setFlying(true);

		}
	}
}

void Character::fire(float loadFireFactor)
{

	if (_ki <= 0.f) return;

	if (_isLookingLeft)
		_attackCollider.setPosition(_position.x - _s.getGlobalBounds().width, _position.y - _s.getGlobalBounds().height * 1.25f);
	else
		_attackCollider.setPosition(_position.x + _s.getGlobalBounds().width / 5.f, _position.y - _s.getGlobalBounds().height * 1.25f);

	if (_target != nullptr)
	{
		std::cout << _name + " : shot a projectile to : " + _target->_name << std::endl;

		sf::CircleShape projectile(10 * loadFireFactor);
		projectile.setFillColor(sf::Color::Yellow);

		if (_isLookingLeft)
			projectile.setPosition(_position.x - _s.getGlobalBounds().width / 2.f, _position.y - _s.getGlobalBounds().height / 2.f);
		else
			projectile.setPosition(_position.x + _s.getGlobalBounds().width / 2.f, _position.y - _s.getGlobalBounds().height / 2.f);

		_projectiles.push_back(projectile);
		useKi(2.f * loadFireFactor);
	}
}

void Character::chargeKI(const float& amount)
{
	_ki += amount;
	if (_ki > 100.f)
		_ki = 100.f;
}

void Character::useKi(const float& amount)
{
	_ki -= amount;
	if (_ki < 0.f)
		_ki = 0.f;
}

void Character::updateState(std::map<std::string, bool>& keyMap)
{
	if (_controllable) {
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
					addForce(-_movingSpeed * 1.09f, 0.f);
				}
				else if (keyMap["Right"])
				{
					_currentAnimationState = (_isLookingLeft ? AnimationStates::AIR_LEFT : AnimationStates::AIR_RIGHT);
					addForce(_movingSpeed * 1.09f, 0.f);
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
			_currentAnimationState = AnimationStates::ATTACK;
			_canMove = false;
		}

		if (keyMap["Fire"])
		{
			_currentAnimationState = AnimationStates::FIRE;
			_canMove = false;
		}
	}
}

void Character::updatePosition(const float& deltaTime, std::map<std::string, bool>& keyMap)
{
	sf::Vector2f friction = -1.f * Utils::normalize(_velocity) * Utils::magnitude(_velocity) * COEF_FRICTION;

	if (!_isFlying)
		addForce(0.f, GRAVITY * GRAVITY_MULTIPLIER * _mass);
	addForce(friction);

	_velocity += _acceleration * deltaTime;
	_position += _velocity;

	_isGrounded = _position.y >= 475;

	if (_position.y >= 500)
	{
		_velocity.y = 0.f;
		_acceleration.y = 0.f;
		_position.y = 500;
		_isFlying = false;
	}

	if (_position.x > 800) {
		//_position.x = 0;
		_velocity.x = -_velocity.x;
	}

	if (_position.x < 0) {
		//_position.x = 800;
		_velocity.x = -_velocity.x;
	}

	if (_isLookingLeft)
		_s.setPosition(_position.x - _s.getGlobalBounds().width, _position.y);
	else
		_s.setPosition(_position);

	_acceleration = sf::Vector2f(0, 0);

}

void Character::updateProjectile(const float& deltaTime)
{
	if (_target == nullptr) {
		_projectiles.clear();
		return;
	}

	for (size_t i = 0; i < _projectiles.size(); ++i)
	{
		if (_target->getCharacterCollider().intersects(_projectiles[i].getGlobalBounds()))
		{

			float damageMultiplier = _projectiles[i].getRadius() / 10.f;

			_target->takeDamage(100.f * damageMultiplier);

			if (!_target->getIsDead())
				_target->playAnimation("Hit_Face_" + std::to_string(rand() % 2 + 1), false, true);

			_projectiles.erase(_projectiles.begin() + i);
			return;
		}

		/*auto targetDir = Utils::direction(_projectiles[i].getPosition(), sf::Vector2f(_target->getPosition().x, _target->getPosition().y - _target->getCharacterCollider().height / 2.f));
		_projectiles[i].move(targetDir * 300.f * deltaTime);*/

		auto targetPos = sf::Vector2f(_target->getPosition().x, _target->getPosition().y - _target->getCharacterCollider().height / 2.f);
		sf::Vector2f newPosition = Utils::vLerp(_projectiles[i].getPosition(), targetPos, 8.f * deltaTime);

		_projectiles[i].setPosition(newPosition);

	}
}

void Character::updateAnimation(const float& deltaTime)
{
	switch (_currentAnimationState)
	{
	case AnimationStates::IDLE:
		if (_isGrounded)
		{
			if (_ki >= 85.f)
				_animator->playAnimation(std::to_string(_currentTransformation) + "_Idle_Powered", true);
			else
				_animator->playAnimation(std::to_string(_currentTransformation) + "_Idle", true);
		}
		else {
			_animator->playAnimation(std::to_string(_currentTransformation) + "_Idle_Fly", true);
		}
		break;
	case AnimationStates::GROUND_LEFT:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Left_Ground");
		break;
	case AnimationStates::GROUND_RIGHT:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Right_Ground");
		break;
	case AnimationStates::AIR_LEFT:
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
		break;
	case AnimationStates::ATTACK:
		_animator->playAnimation(std::to_string(_currentTransformation) + "_Attack_" + std::to_string(_currentComboIndex), false, true);
		_currentComboIndex = (_currentComboIndex + 1) % 5 + 1;

		_attackCollider.setSize(sf::Vector2f(20.f, 50.f));
		_attack = true;
		if (_isLookingLeft)
			_attackCollider.setPosition(_position.x - _s.getGlobalBounds().width, _position.y - _s.getGlobalBounds().height);
		else
			_attackCollider.setPosition(_position.x + _s.getGlobalBounds().width / 2.f, _position.y - _s.getGlobalBounds().height);

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
	case AnimationStates::KO:
		_animator->playAnimation("KO");
		break;
	case AnimationStates::FIRE:
		_animator->playAnimation("Fire", false, true);
		break;
	}

	_animator->update(deltaTime);
}

