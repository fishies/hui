#pragma once

#include "ecs.hpp"

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

    void tick();
};

class InputSystem : System
{
public:
    InputSystem(EntityManager* entityAdmin);
    
    void tick();
};