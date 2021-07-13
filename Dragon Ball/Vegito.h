#pragma once
#include "Character.h"
class Vegito :
    public Character
{
public:

    Vegito();

    // Inherited via Character
    virtual void update(const float& deltaTime, std::map<std::string, bool>& keyMap) override;
    virtual void render(sf::RenderTarget* target) override;
    virtual void init() override;

private:

    sf::Text _vegitoUI;


    // Inherited via Character
    virtual void updateState(std::map<std::string, bool>& keyMap) override;
    virtual void updatePosition(const float& deltaTime, std::map<std::string, bool>& keyMap) override;
    virtual void updateAnimation(const float& deltaTime) override;


};

