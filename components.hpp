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

struct PlayerController : Component
{
    bool inGame;

    PlayerController()
    : Component("PlayerController"), inGame(false)
    {}
};

struct Transform : Component
{
    sf::Transformable* transformable;

    Transform(sf::Transformable* transformable)
    : Component("Transform"), transformable(transformable)
    {}
};

struct Velocity : Component
{
    float x;
    float y;
    
    Velocity(int v_x = 0.0f, int v_y = 0.0f)
    : Component("Velocity"), x(v_x), y(v_y)
    {}
};

struct Collider : Component
{
    sf::Shape* shape;

    Collider(sf::Shape* shape)
    : Component("Collider"), shape(shape)
    {}
};

struct Shader : Component
{
    sf::Shader* shader;
    float* strength;

    Shader(sf::Shader* shader, float* strength)
    : Component("Shader"), shader(shader), strength(strength)
    {}
};

struct Home : Component
{
    Home()
    : Component("Home")
    {}
};

struct Tutorial : Component
{
    Tutorial()
    : Component("Tutorial")
    {}
};

struct Level : Component
{
    Level()
    : Component("Level")
    {}
};
