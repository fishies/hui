#include "components.hpp"
#include "systems.hpp"
#include "obb.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

void addHWall(EntityManager &entityManager, const sf::VideoMode &screenSize, sf::Shader *shader, float *hitMag, int x, int y)
{
    if(y == 0) return;
    sf::RectangleShape* wall = new sf::RectangleShape();
    wall->setFillColor(sf::Color::White);
    wall->move(screenSize.width*(5.0f/100.0f),screenSize.height*(5.0f/100.0f));
    wall->move(screenSize.width*(10.0f/100.0f)*x,screenSize.height*(12.85715f/100.0f)*y);
    wall->setSize(sf::Vector2f(screenSize.width*(10.0f/100.0f),
                               screenSize.height*(2.0f/100.0f)));
    entityManager.addEntity({new Drawable(wall),
                             new Transform(wall),
                             new Collider(wall),
                             new Shader(shader, hitMag),
                             new Level()});
}

void addVWall(EntityManager &entityManager, const sf::VideoMode &screenSize, sf::Shader *shader, float *hitMag, int x, int y)
{
    sf::RectangleShape* wall = new sf::RectangleShape();
    wall->setFillColor(sf::Color::White);
    wall->move(screenSize.width*(4.0f/100.0f),screenSize.height*(5.0f/100.0f));
    if(x > 7) x = 7;
    wall->move(screenSize.width*(10.0f/100.0f)*(x+1),screenSize.height*(12.85715f/100.0f)*y);
    wall->setSize(sf::Vector2f(screenSize.width*(2.0f/100.0f),
                               screenSize.height*(12.85715f/100.0f)));
    entityManager.addEntity({new Drawable(wall),
                             new Transform(wall),
                             new Collider(wall),
                             new Shader(shader, hitMag),
                             new Level()});
}

void generateMaze(EntityManager &entityManager, const sf::VideoMode &screenSize, sf::Shader *shader, float *hitMag, const std::string &dna)
{
    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 7; ++j)
        {
            if((dna.at(i+j*9)-'0')&1)
                addVWall(entityManager, screenSize, shader, hitMag, i, j);
            if((dna.at(i+j*9)-'0')&2)
                addHWall(entityManager, screenSize, shader, hitMag, i, j);
        }
    }
}

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

Reset::Reset(EntityManager *entityAdmin) : System(entityAdmin)
{
    subscribe("Home");
    subscribe("Tutorial");
    subscribe("PlayerController");
    subscribe("Level");
}

#include <iostream>

void Reset::tick()
{
    bool reset = false;
    for(auto itr = entitiesWithComponent["Home"].begin();
        itr != entitiesWithComponent["Home"].end(); ++itr)
    {
        Home* home = (Home*)(entityManager->getComponent(*itr,"Home"));
        if(home == nullptr) continue;
        for(auto jtr = entitiesWithComponent["PlayerController"].begin();
            jtr != entitiesWithComponent["PlayerController"].end(); ++jtr)
        {
            sf::Shape* pcc = ((Collider*)(entityManager->getComponent(*jtr,"Collider")))->shape;
            if(((PlayerController*)(entityManager->getComponent(*jtr,"PlayerController"))) != nullptr &&
               !((PlayerController*)(entityManager->getComponent(*jtr,"PlayerController")))->inGame)
            {
                return;
            }
            
            if(pcc == nullptr) continue;

            sf::RectangleShape* box1 = (home->home);
            sf::RectangleShape box2;
            box2.setPosition(pcc->getGlobalBounds().left,pcc->getGlobalBounds().top);
            box2.setSize(sf::Vector2f(pcc->getGlobalBounds().width,pcc->getGlobalBounds().height));
            sf::Vector2f mtv;

            if(testCollision(*box1,box2,mtv))
            {
                reset = true;
            }
        }
    }
    if(reset)
    {
        //make tutorial visible
        for(auto itr = entitiesWithComponent["Tutorial"].begin();
            itr != entitiesWithComponent["Tutorial"].end(); ++itr)
        {
            Drawable* tutUI = (Drawable*)(entityManager->getComponent(*itr,"Drawable"));
            if(tutUI != nullptr) tutUI->visible = true;
        }
        //reset player controller and position of player
        for(auto itr = entitiesWithComponent["PlayerController"].begin();
            itr != entitiesWithComponent["PlayerController"].end(); ++itr)
        {
            ((PlayerController*)(entityManager->getComponent(*itr,"PlayerController")))->inGame = false;
            
            Transform* playerTransform = (Transform*)(entityManager->getComponent(*itr,"Transform"));
            if(playerTransform != nullptr)
            {
                playerTransform->transformable->setPosition(xOrigin,yOrigin);
            }
        }
        //erase level
        for(;entitiesWithComponent["Level"].size() > 0;)
        {
            entityManager->removeEntity(*(entitiesWithComponent["Level"].begin()));
        }
    }
}

MovementSystem::MovementSystem(EntityManager *entityAdmin) : System(entityAdmin)
{
    subscribe("Transform");
    subscribe("Velocity");
    subscribe("Collider");
    subscribe("PlayerController");
    subscribe("Tutorial");
}

void MovementSystem::tick()
{
    const float dv = 0.125f;
    const float bounce = -1.25f;
    const float vcap = 6.0f;
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

            float* strength;

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
                strength = shader->strength;

                if(testCollision(obb1,obb2,mtv)) // process collision result
                {
                    PlayerController* pc = ((PlayerController*)(entityManager->getComponent(*itr,"PlayerController")));
                    if(pc != nullptr && !(pc->inGame))
                    {
                        pc->inGame = true;
                        generateMaze(*entityManager, screenSize, shdr, hitMag,
                        "000100010032122310202313010122001230112101100212013030030300220");
                        for(auto ktr = entitiesWithComponent["Tutorial"].begin();
                            ktr != entitiesWithComponent["Tutorial"].end(); ++ktr)
                        {
                            Drawable* viz = (Drawable*)(entityManager->getComponent(*ktr,"Drawable"));
                            if(viz == nullptr) continue;
                            viz->visible = false;
                        }
                    }

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
                    *strength = 100.0f;

                    break;
                }

                if(shader == nullptr) continue;
                shader->shader->setUniform("strength", *strength);
            }
            
            *strength -= 4.0f;
            if(*strength < 0.0f) *strength = 0.0f;
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
    float dv = 1.375f;
    const float vcap = 5.0f;
    for(auto itr = entitiesWithComponent["PlayerController"].begin();
        itr != entitiesWithComponent["PlayerController"].end(); ++itr)
    {
        Velocity* v = (Velocity*)(entityManager->getComponent(*itr,"Velocity"));
        if(((PlayerController*)(entityManager->getComponent(*itr,"PlayerController")))->inGame)
        {
            dv = 1.375f;
        }
        else
        {
            dv = 25.0f;
        }
        
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