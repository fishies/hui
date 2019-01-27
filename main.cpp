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
    float hitMag = 0.0f;

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
    player.move(screenSize.width/2 - player.getSize().x/2,
                screenSize.height/2 - player.getSize().y/2);

    
    // Player Avatar
    entityManager.addEntity({new Drawable(&player),
                             new Velocity(0.0f,0.0f),
                             new Transform(&player),
                             new PlayerController(),
                             new Collider(&player)});

    // Outer Wall North
    sf::RectangleShape nwall;
    nwall.setFillColor(sf::Color::Black);
    nwall.move(screenSize.width*(2.5f/100.0f),
               screenSize.height*(2.5f/100.0f));
    nwall.setSize(sf::Vector2f(screenSize.width*(95.0f/100.0f),
                               screenSize.height*(2.5f/100.0f)));

    entityManager.addEntity({new Drawable(&nwall),
                             new Transform(&nwall),
                             new Collider(&nwall),
                             new Shader(&shader, &hitMag)});

    // Outer Wall South
    sf::RectangleShape swall;
    swall.setFillColor(sf::Color::Black);
    swall.move(screenSize.width*(2.5f/100.0f),
               screenSize.height*(95.0f/100.0f));
    swall.setSize(sf::Vector2f(screenSize.width*(95.0f/100.0f),
                               screenSize.height*(2.5f/100.0f)));

    entityManager.addEntity({new Drawable(&swall),
                             new Transform(&swall),
                             new Collider(&swall),
                             new Shader(&shader, &hitMag)});

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
