#include "components.hpp"
#include "systems.hpp"
#include <SFML/Graphics.hpp>

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
        Position* position = (Position*)(entityManager->getComponent(*itr,"Position"));
        if(current != nullptr && current->visible)
        {
            /*if(position != nullptr)
            {
                current->drawable.setPosition((position->x)*(target->getSize().x),
                                            (position->y)*(target->getSize().y));
            }*/
            target->draw(*(current->drawable));
        }
    }
}
