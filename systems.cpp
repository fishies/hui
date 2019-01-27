#include "components.hpp"
#include "systems.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

DrawSystem::DrawSystem(EntityManager* entityAdmin, sf::RenderWindow* target) : System(entityAdmin), target(target)
{
    subscribe("Drawable");
}

void DrawSystem::tick()
{
    //clear and display should be called before and after this is called
    for(auto itr = entitiesWithComponent["Drawable"].begin();
        itr != entitiesWithComponent["Drawable"].end(); ++itr)
    {
        Drawable* current = (Drawable*)(entityManager->getComponent(*itr,"Drawable"));
        if(current != nullptr && current->visible)
        {
            target->draw(*(current->drawable));
        }
    }
}

MovementSystem::MovementSystem(EntityManager *entityAdmin) : System(entityAdmin)
{
    subscribe("WorldPosition");
    subscribe("Transform");
    subscribe("Velocity");
}

void MovementSystem::tick()
{
    const float dv = 0.125f;
    for(auto itr = entitiesWithComponent["Velocity"].begin();
        itr != entitiesWithComponent["Velocity"].end(); ++itr)
    {
        Velocity* v = (Velocity*)(entityManager->getComponent(*itr,"Velocity"));
        Transform* transform = (Transform*)(entityManager->getComponent(*itr,"Transform"));
        //WorldPosition* worldPos = (WorldPosition*)(entityManger->getComponent(*itr,"WorldPosition"));
        if(v != nullptr && transform != nullptr /*&& worldPost != nullptr*/)
        {
            transform->transformable->move(v->x, v->y);
            
            if(v->x > 0.0f)
            {
                if((v->x -= dv) < 0.0f)
                    v->x = 0.0f;
            }
            else
            {
                if((v->x += dv) > 0.0f)
                    v->x = 0.0f;
            }

            if(v->y > 0.0f)
            {
                if((v->y -= dv) < 0.0f)
                    v->y = 0.0f;
            }
            else
            {
                if((v->y += dv) > 0.0f)
                    v->y = 0.0f;
            }
        }
    }
}

InputSystem::InputSystem(EntityManager *entityAdmin) : System(entityAdmin)
{
    subscribe("PlayerController");
    subscribe("Velocity");
}

void InputSystem::tick()
{
    const float dv = 1.375f;
    const float vcap = 5.0f;
    for(auto itr = entitiesWithComponent["PlayerController"].begin();
        itr != entitiesWithComponent["PlayerController"].end(); ++itr)
    {
        Velocity* v = (Velocity*)(entityManager->getComponent(*itr,"Velocity"));
        if(v != nullptr)
        {
            float vx = 0.0f;
            float vy = 0.0f;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && v->y > -vcap)
            {
                vy -= dv;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && v->y < vcap)
            {
                vy += dv;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && v->x > -vcap)
            {
                vx -= dv;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && v->x < vcap)
            {
                vx += dv;
            }

            float magnitude = sqrt((vx*vx+vy*vy));
            if(!magnitude) continue;
            vx /= magnitude;
            vy /= magnitude;
            vx *= dv;
            vy *= dv;

            v->x += vx;
            v->y += vy;
        }
    }
}