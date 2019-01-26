#pragma once

#include "ecs.hpp"

#include <string>
#include <SFML/Graphics.hpp>

struct Drawable : Component
{
    sf::Drawable* drawable;
    int layer;
    bool visible;

    Drawable(sf::Drawable* drawable, int layer = 0, bool visible = true)
    : Component("Drawable"), drawable(drawable), layer(layer), visible(visible)
    {}
};

struct Position : Component
{
    float x;
    float y;

    Position(float x = 0.0f, float y = 0.0f)
    : Component("Position"), x(x), y(y)
    {}
};

struct Velocity : Component
{
    int x;
    int y;
    
    Velocity(int v_x = 0, int v_y = 0)
    : Component("Velocity"), x(v_x), y(v_y)
    {}
};

struct Collision : Component
{
    //point of incidence
    int x;
    int y;
    
    Collision(int c_x = 0, int c_y = 0)
    : Component("Collision"), x(c_x), y(c_y)
    {}
};
