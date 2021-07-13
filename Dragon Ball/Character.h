#pragma once

#include "Animator.h"

class Character
{
public:

	Character()
	{
		_animator = new Animator(_s);
	}

	~Character()
	{
		delete(_animator);
	}


	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget* target) = 0;

protected:

	sf::Sprite _s;
	Animator* _animator;

	sf::Vector2f _acceleration, _velocity, _position, _friction;
	std::string _name;
	float _mass = 10.f;

};

