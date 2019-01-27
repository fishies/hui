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
    MovementSystem movementSystem(&entityManager);
    InputSystem inputSystem(&entityManager);

    sf::Clock clock;
    const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Black);

    sf::RectangleShape wall;
    wall.setFillColor(sf::Color::Black);
    wall.move(400.f,400.f);
    wall.setSize(sf::Vector2f(200.f,200.f));

    entityManager.addEntity({new Drawable(&shape),
                             new Velocity(0.0f,0.0f),
                             new Transform(&shape),
                             new PlayerController(),
                             new Collider(&shape)});

    entityManager.addEntity({new Drawable(&wall),
                             new Transform(&wall),
                             new Collider(&wall)});

    for(;
        window.isOpen();
        )
    {
        sf::Time dt = clock.restart();
        for(timeSinceLastUpdate += dt;
            timeSinceLastUpdate > timePerFrame;
            timeSinceLastUpdate -= timePerFrame)
        {
            window.clear(sf::Color::White);
            drawSystem.tick();
            inputSystem.tick();
            movementSystem.tick();
            window.display();
        }

        for(sf::Event event;window.pollEvent(event);)
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
    }
    return 0;
}
