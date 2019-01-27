#include "ecs.hpp"
#include "components.hpp"
#include "systems.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    EntityManager entityManager;
    sf::VideoMode screenSize = sf::VideoMode::getFullscreenModes()[0];
    sf::RenderWindow window(screenSize, "hui", sf::Style::Fullscreen);
    

    sf::Shader shader;
    shader.loadFromFile("hit.frag", sf::Shader::Fragment);
    shader.setUniform("screenHeight",(float)((int)(screenSize.height)));

    DrawSystem drawSystem(&entityManager, &window);
    MovementSystem movementSystem(&entityManager);
    InputSystem inputSystem(&entityManager);

    sf::Clock clock;
    const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    sf::Clock shaderClock;

    sf::RectangleShape player;
    player.setFillColor(sf::Color::Black);
    player.setSize(sf::Vector2f(screenSize.width*(1.0f/50.0f),
                                screenSize.width*(1.0f/50.0f)));
    player.move(screenSize.width/2 + player.getSize().x/2,
                screenSize.height/2 + player.getSize().y/2);

    sf::RectangleShape wall;
    wall.setFillColor(sf::Color::Black);
    wall.move(400.f,400.f);
    wall.setSize(sf::Vector2f(1000.f,600.f));

    // Player Avatar
    entityManager.addEntity({new Drawable(&player),
                             new Velocity(0.0f,0.0f),
                             new Transform(&player),
                             new PlayerController(),
                             new Collider(&player)});

    // Outer Wall 1
    entityManager.addEntity({new Drawable(&wall),
                             new Transform(&wall),
                             new Collider(&wall),
                             new Shader(&shader)});

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
            shader.setUniform("time",shaderClock.getElapsedTime().asSeconds());
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
