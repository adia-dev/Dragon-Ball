#include <SFML/Graphics.hpp>

constexpr uint32_t W = 800;
constexpr uint32_t H = 600;

int main()
{
    sf::RenderWindow window(sf::VideoMode(W, H), "SFML TEMPLATE!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(W / 2.f - shape.getGlobalBounds().width / 2.f, H / 2.f - shape.getGlobalBounds().height / 2.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                }
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}