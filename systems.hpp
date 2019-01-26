#pragma once

#include "ecs.hpp"

class DrawSystem : System
{
    sf::RenderWindow* target;

public:
    DrawSystem(EntityManager* entityAdmin, sf::RenderWindow* target);

    void tick();
};
