#pragma once
#include "Character.h"
class Vegito :
    public Character
{
public:

    Vegito();

    // Inherited via Character
    virtual void update(const float& deltaTime) override;

    virtual void render(sf::RenderTarget* target) override;

private:

    


};

