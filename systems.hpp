#pragma once

#include "ecs.hpp"

void addHWall(EntityManager &entityManager, const sf::VideoMode &screenSize, sf::Shader *shader, float *hitMag, int x, int y);
void addVWall(EntityManager &entityManager, const sf::VideoMode &screenSize, sf::Shader *shader, float *hitMag, int x, int y);
void generateMaze(EntityManager &entityManager, const sf::VideoMode &screenSize, sf::Shader *shader, float *hitMag, const std::string &dna);

class DrawSystem : System
{
    sf::RenderWindow* target;

public:
    DrawSystem(EntityManager* entityAdmin, sf::RenderWindow* target);

    void tick();
};

class MovementSystem : System
{
public:
    MovementSystem(EntityManager* entityAdmin);

/*spaghetti*/
    sf::VideoMode screenSize;
    sf::Shader* shdr;
    float* hitMag;
/* :( */

    void tick();
};

class InputSystem : System
{
public:
    InputSystem(EntityManager* entityAdmin);
    
    void tick();
};

class Reset : System
{
public:
    Reset(EntityManager* entityAdmin);

    float xOrigin;
    float yOrigin;

    void tick();
};