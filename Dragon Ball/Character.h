#pragma once

#include "Animator.h"

#define GRAVITY 9.81f
#define GRAVITY_MULTIPLIER 0.5f
#define COEF_FRICTION 100.f

enum class AnimationStates
{
	IDLE,
	GROUND_LEFT,
	GROUND_RIGHT,
	AIR_LEFT,
	AIR_RIGHT,
	UP,
	DOWN,
	SLIDE,
	TELEPORT,
	ATTACK,
	SPECIAL_1,
	SPECIAL_2,
	TRANSFORM,
	HIT,
	GUARD,
	CHARGE,
	TAUNT
};

class Character
{
public:

	Character();

	~Character();


	virtual void init();

	virtual void update(const float& deltaTime, std::map<std::string, bool>& keyMap);


	virtual void render(sf::RenderTarget* target);

	void takeControl() { _controllable = true; }
	void releaseControl() { _controllable = false; }

	void addForce(const sf::Vector2f& force);
	void addForce(const float& x, const float& y);

	void updateCharacterUI();

	void playAnimation(const std::string& name, bool isLoop, bool isLocked)
	{
		_animator->playAnimation(name, isLoop, isLocked);
	}

	void toggleUI() { _renderUI = !_renderUI; }
	void toggleFly()
	{
		if (!_isGrounded)
			_isFlying = !_isFlying;
	}

	void setTarget(Character* target)
	{
		_target = target;
	}

	const Character* getTarget()
	{
		return _target;
	}

	const std::string& getName() {
		return _name;
	}

	const sf::Vector2f& getPosition()
	{
		return _position;
	}

protected:

	void chargeKI(const float& amount)
	{
		_ki += amount;
		if (_ki > 100.f)
			_ki = 100.f;
	}

	void useKi(const float& amount)
	{
		_ki -= amount;
		if (_ki < 0.f)
			_ki = 0.f;
	}

	virtual void updateState(std::map<std::string, bool>& keyMap);
	virtual void updatePosition(const float& deltaTime, std::map<std::string, bool>& keyMap);
	virtual void updateAnimation(const float& deltaTime);

	sf::Sprite _s;
	sf::CircleShape _originUI;
	sf::RectangleShape _attackCollider;
	sf::RectangleShape _groundCastUI;
	sf::RectangleShape _hpUI;
	sf::RectangleShape _kiUI;

	Animator* _animator;
	AnimationStates _currentAnimationState = AnimationStates::IDLE;
	std::string _name = "Default Character";

	sf::Vector2f _acceleration, _velocity, _position, _friction;

	Character* _target = nullptr;

	float _mass = 10.f;
	float _movingSpeed = 75.f;
	float _attackTimer = 0.f;
	float _attackDelay = 0.4f;
	float _ki = 10.f;
	float _hp = 5000.f;

	int _currentComboIndex = 0;
	int _hitCount = 0;

	bool _isFlying = false;
	bool _isGrounded = false;
	bool _controllable = false;
	bool _renderUI = false;
	bool _charged = false;
	bool _isLookingLeft = false;
	bool _canMove = true;
	bool _attack = false;

};

