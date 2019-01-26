#include "ecs.hpp"
#include "components.hpp"
#include "systems.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    EntityManager entityManager;
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0],
                            "hui", sf::Style::Fullscreen);

    DrawSystem drawSystem(&entityManager, &window);
    
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Black);
    entityManager.addEntity({new Drawable(&shape)});

    for(sf::Event event;
        window.isOpen();
        )
    {
        for(;window.pollEvent(event);)
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }
        window.clear(sf::Color::White);
        drawSystem.tick();
        window.display();
    }
    return 0;
}
