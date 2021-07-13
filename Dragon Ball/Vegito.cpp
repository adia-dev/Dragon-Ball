#include "Vegito.h"

Vegito::Vegito()
{
	_s.setTexture(AssetManager::GetTexture("vegito_blue.png", PATHS::CHARACTERS));

	_animator->loadAnimation("vegito_blue.xml", PATHS::ANIMATIONS);

	_s.setPosition(400, 300);

}

void Vegito::update(const float& deltaTime)
{
}

void Vegito::render(sf::RenderTarget* target)
{
	if (target == nullptr) return;

	target->draw(_s);
}
