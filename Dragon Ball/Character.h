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
	TAUNT,
	RUSHING,
	KO,
	FIRE
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

	void playAnimation(const std::string& name, bool isLoop, bool isLocked);

	void takeDamage(float amount);

	bool getIsDead() {
		return _isDead;
	}

	void attack(float chargeAttackFactor);
	void fire(float loadFireFactor);

	void setFlying(bool state)
	{
		_isFlying = state;
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

	bool isGrounded()
	{
		return _isGrounded;
	}

	sf::FloatRect getCharacterCollider()
	{
		return _characterCollider.getGlobalBounds();
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

	int getCurrentTransformation()
	{
		return _currentTransformation;
	}

protected:

	void chargeKI(const float& amount);

	void useKi(const float& amount);

	virtual void updateState(std::map<std::string, bool>& keyMap);
	virtual void updatePosition(const float& deltaTime, std::map<std::string, bool>& keyMap);
	virtual void updateProjectile(const float& deltaTime);
	virtual void updateAnimation(const float& deltaTime);

	sf::Sprite _s;
	sf::Sprite _headshot;
	sf::CircleShape _originUI;
	sf::RectangleShape _attackCollider;
	sf::RectangleShape _characterCollider;
	sf::RectangleShape _groundCastUI;
	sf::RectangleShape _hpUI;
	sf::RectangleShape _kiUI;

	Animator* _animator;
	AnimationStates _currentAnimationState = AnimationStates::IDLE;
	std::string _name = "Default Character";

	sf::Vector2f _acceleration, _velocity, _position, _friction;

	Character* _target = nullptr;

	float _mass = 10.f;
	float _movingSpeed = 45.f;
	float _attackTimer = 0.f;
	float _attackDelay = 0.4f;
	float _colliderDebugTimer = 0.f;
	float _ki = 100.f;
	float _hp = 5000.f;

	int _currentComboIndex = 1;
	int _hitCount = 0;
	int _currentTransformation = 0;

	bool _isFlying = false;
	bool _isGrounded = false;
	bool _controllable = false;
	bool _renderUI = false;
	bool _charged = false;
	bool _isLookingLeft = false;
	bool _canMove = true;
	bool _attack = false;
	bool _rushing = false;
	bool _isDead = false;

	std::vector<std::string> _animationStatesNames{
	"IDLE",
	"GROUND_LEFT",
	"GROUND_RIGHT",
	"AIR_LEFT",
	"AIR_RIGHT",
	"UP",
	"DOWN",
	"SLIDE",
	"TELEPORT",
	"ATTACK",
	"SPECIAL_1",
	"SPECIAL_2",
	"TRANSFORM",
	"HIT",
	"GUARD",
	"CHARGE",
	"TAUNT",
	"RUSHING",
	"KO",
	"FIRE"
	};
	std::vector<sf::CircleShape> _projectiles;
};

