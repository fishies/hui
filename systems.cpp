#include "components.hpp"
#include "systems.hpp"
#include "obb.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

#include <iostream>

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
        Shader* shader = ((Shader*)(entityManager->getComponent(*itr,"Shader")));
        if(current != nullptr && current->visible)
        {
            if(shader != nullptr)
                target->draw(*(current->drawable),shader->shader);
            else
                target->draw(*(current->drawable));
        }
    }
}

MovementSystem::MovementSystem(EntityManager *entityAdmin) : System(entityAdmin)
{
    subscribe("WorldPosition");
    subscribe("Transform");
    subscribe("Velocity");
    subscribe("Collider");
}

void MovementSystem::tick()
{
    const float dv = 0.125f;
    const float bounce = -1.25f;
    const float vcap = 8.0f;
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

            // collision detection
            //convert bounding box to rectangleshape
            sf::RectangleShape obb1;
            sf::Shape* shPtr = ((Collider*)(entityManager->getComponent(*itr,"Collider")))->shape;
            if(shPtr == nullptr) continue;
            obb1.setPosition(shPtr->getGlobalBounds().left,shPtr->getGlobalBounds().top);
            obb1.setSize(sf::Vector2f(shPtr->getGlobalBounds().width,shPtr->getGlobalBounds().height));

            for(auto jtr = entitiesWithComponent["Collider"].begin();
                jtr != entitiesWithComponent["Collider"].end(); ++jtr)
            {
                sf::RectangleShape obb2;
                sf::Shape* shaPtr = ((Collider*)(entityManager->getComponent(*jtr,"Collider")))->shape;
                if(shaPtr == nullptr) continue;
                if(shaPtr == shPtr) continue;
                obb2.setPosition(shaPtr->getGlobalBounds().left,shaPtr->getGlobalBounds().top);
                obb2.setSize(sf::Vector2f(shaPtr->getGlobalBounds().width,shaPtr->getGlobalBounds().height));

                sf::Vector2f mtv;
                Shader* shader = ((Shader*)(entityManager->getComponent(*jtr,"Shader")));

                if(testCollision(obb1,obb2,mtv)) // process collision result
                {
                    transform->transformable->move(mtv);
                    obb1.move(mtv);
                    v->x *= bounce;
                    v->y *= bounce;

                    if(v->x > vcap) v->x = vcap;
                    if(v->x < -vcap) v->x = -vcap;
                    if(v->y > vcap) v->y = vcap;
                    if(v->y < -vcap) v->y = -vcap;

                    if(shader == nullptr) continue;
                    shader->shader->setUniform("source",
                            sf::Vector2f(obb1.getPosition().x + obb1.getSize().x*0.5f,
                                         obb1.getPosition().y + obb1.getSize().y*0.5f));
                    *(shader->strength) = 100.0f;

                    break;
                }

                if(shader == nullptr) continue;
                shader->shader->setUniform("strength", *(shader->strength));
                *(shader->strength) -= 1.0f;
                if(*(shader->strength) < 0.0f) *(shader->strength) = 0.0f;
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