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
    Reset reset(&entityManager);

    sf::Clock clock;
    const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    sf::Clock shaderClock;

    movementSystem.screenSize = screenSize;
    movementSystem.shdr = &shader;
    movementSystem.hitMag = &hitMag;

    // Player Avatar
    sf::RectangleShape player;
    player.setFillColor(sf::Color::Black);
    player.setSize(sf::Vector2f(screenSize.width*(1.0f/50.0f),
                                screenSize.width*(1.0f/50.0f)));
    player.move(screenSize.width/2 - player.getSize().x/2,
                screenSize.height/2 - player.getSize().y/2);
    
    reset.xOrigin = screenSize.width/2 - player.getSize().x/2;
    reset.yOrigin = screenSize.height/2 - player.getSize().y/2;

    entityManager.addEntity({new Drawable(&player),
                             new Velocity(0.0f,0.0f),
                             new Transform(&player),
                             new PlayerController(),
                             new Collider(&player)});

    // Home
    sf::RectangleShape home;
    home.setFillColor(sf::Color::White);
    home.setOutlineColor(sf::Color::Black);
    home.setOutlineThickness(-36.0f);
    home.setSize(sf::Vector2f(screenSize.width*(12.0f/100.0f),
                              screenSize.height*(12.0f/100.0f)));
    home.move(screenSize.width/2 - home.getSize().x/2,
              screenSize.height/2 - home.getSize().y/2);

    entityManager.addEntity({new Drawable(&home)/*,
                             new Home()*/});

    // Controls
    sf::RectangleShape upBox, downBox, leftBox, rightBox;
    sf::CircleShape upArrow(screenSize.height*(1.5f/100.0f), 3),
                    downArrow(screenSize.height*(1.5f/100.0f), 3),
                    leftArrow(screenSize.height*(1.5f/100.0f), 3),
                    rightArrow(screenSize.height*(1.5f/100.0f), 3);
    upBox.setFillColor(sf::Color::Transparent);
    upBox.setOutlineColor(sf::Color::Black);
    upBox.setOutlineThickness(-12.0f);
    upBox.setSize(sf::Vector2f(screenSize.height*(5.0f/100.0f),
                               screenSize.height*(5.0f/100.0f)));
    upBox.move(screenSize.width/2 - screenSize.height*(2.5f/100.0f),
               screenSize.height*(69.0f/100.0f));
    entityManager.addEntity({new Drawable(&upBox), new Tutorial()});
    upArrow.setFillColor(sf::Color::Black);
    upArrow.move(screenSize.width/2 - screenSize.height*(1.5f/100.0f), screenSize.height*(70.25f/100.0f));
    entityManager.addEntity({new Drawable(&upArrow), new Tutorial()});

    downBox.setFillColor(sf::Color::Transparent);
    downBox.setOutlineColor(sf::Color::Black);
    downBox.setOutlineThickness(-12.0f);
    downBox.setSize(sf::Vector2f(screenSize.height*(5.0f/100.0f),
                                 screenSize.height*(5.0f/100.0f)));
    downBox.move(screenSize.width/2 - screenSize.height*(2.5f/100.0f),
                 screenSize.height*(75.0f/100.0f));
    entityManager.addEntity({new Drawable(&downBox), new Tutorial()});

    downArrow.setFillColor(sf::Color::Black);
    downArrow.setRotation(180);
    downArrow.move(screenSize.width/2 + screenSize.height*(1.5f/100.0f),
                   screenSize.height*(78.75f/100.0f));
    entityManager.addEntity({new Drawable(&downArrow), new Tutorial()});
    
    leftBox.setFillColor(sf::Color::Transparent);
    leftBox.setOutlineColor(sf::Color::Black);
    leftBox.setOutlineThickness(-12.0f);
    leftBox.setSize(sf::Vector2f(screenSize.height*(5.0f/100.0f),
                                 screenSize.height*(5.0f/100.0f)));
    leftBox.move(screenSize.width/2 - screenSize.height*(8.5f/100.0f),
                 screenSize.height*(75.0f/100.0f));
    entityManager.addEntity({new Drawable(&leftBox), new Tutorial()});

    leftArrow.setFillColor(sf::Color::Black);
    leftArrow.move(screenSize.width/2 + screenSize.height*(-7.25f/100.0f),
                   screenSize.height*(79.0f/100.0f));
    leftArrow.setRotation(270);
    entityManager.addEntity({new Drawable(&leftArrow), new Tutorial()});
    
    rightBox.setFillColor(sf::Color::Transparent);
    rightBox.setOutlineColor(sf::Color::Black);
    rightBox.setOutlineThickness(-12.0f);
    rightBox.setSize(sf::Vector2f(screenSize.height*(5.0f/100.0f),
                                  screenSize.height*(5.0f/100.0f)));
    rightBox.move(screenSize.width/2 + screenSize.height*(3.5f/100.0f),
                  screenSize.height*(75.0f/100.0f));
    entityManager.addEntity({new Drawable(&rightBox), new Tutorial()});

    rightArrow.setFillColor(sf::Color::Black);
    rightArrow.move(screenSize.width/2 + screenSize.height*(7.25f/100.0f),
                    screenSize.height*(76.0f/100.0f));
    rightArrow.setRotation(90);
    entityManager.addEntity({new Drawable(&rightArrow), new Tutorial()});

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

    // Outer Wall West
    sf::RectangleShape wwall;
    wwall.setFillColor(sf::Color::Black);
    wwall.move(screenSize.width*(2.5f/100.0f),
               screenSize.height*(2.5f/100.0f));
    wwall.setSize(sf::Vector2f(screenSize.width*(2.5f/100.0f),
                               screenSize.height*(95.0f/100.0f)));

    entityManager.addEntity({new Drawable(&wwall),
                             new Transform(&wwall),
                             new Collider(&wwall),
                             new Shader(&shader, &hitMag)});

    // Outer Wall East
    sf::RectangleShape ewall;
    ewall.setFillColor(sf::Color::Black);
    ewall.move(screenSize.width*(95.0f/100.0f),
               screenSize.height*(2.5f/100.0f));
    ewall.setSize(sf::Vector2f(screenSize.width*(2.5f/100.0f),
                               screenSize.height*(95.0f/100.0f)));

    entityManager.addEntity({new Drawable(&ewall),
                             new Transform(&ewall),
                             new Collider(&ewall),
                             new Shader(&shader, &hitMag)});

//    generateMaze(entityManager, screenSize, &shader, &hitMag,
//    "000100010032122310202313010122001230112101100212013030030300220");
//   0        1        2        3        4        5        6        
//  "333333333333333333333333333333333333333333333333333333333333333"
// 1 vertical
// 2 horizontal

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
            reset.tick();
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
