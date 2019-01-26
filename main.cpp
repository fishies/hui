#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0],
                            "hui", sf::Style::Fullscreen);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Black);

    for(;window.isOpen();)
    {
        window.clear(sf::Color::White);
        window.draw(shape);
        window.display();
    }
    return 0;
}

