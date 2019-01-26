#include "ecs.hpp"

Component::Component(const std::string &name) : type(name) {}

System::System(EntityManager* entityAdmin) : entityManager(entityAdmin) {}

void System::subscribe(const std::string &componentType)
{
    entityManager->addSubscriber(componentType, this);
}

EntityManager::EntityManager() : idPool({1}), highestID(1) {}

entityID EntityManager::addEntity(const std::initializer_list<Component*> &components)
{
    entityID id = idPool.front();
    //assert(!entities.count(id));
    //entities[id] = components;
    for(auto itr = components.begin(); itr != components.end(); ++itr)
    {
        entities[id].push_back(std::unique_ptr<Component>(*itr));
        for(auto jtr = subscribers[(*itr)->type].begin(); jtr != subscribers[(*itr)->type].end(); ++jtr)
        {
            (*jtr)->entitiesWithComponent[(*itr)->type].insert(id);
        }
    }
    idPool.pop();
    if(!idPool.size())
        idPool.push(++highestID);
    return id;
}

bool EntityManager::removeEntity(entityID id)
{
    if(entities.count(id))
    {
        for(auto itr = entities[id].begin(); itr != entities[id].end(); ++itr)
        {
            for(auto jtr = subscribers[(*itr)->type].begin(); jtr != subscribers[(*itr)->type].end(); ++jtr)
            {
                (*jtr)->entitiesWithComponent[(*itr)->type].erase(id);
            }
        }
        entities.erase(id);
        idPool.push(id);
        return true;
    }
    return false;
}

bool EntityManager::addComponent(entityID id, Component* component)
{
    if(!entities.count(id))
        return false;
    entities[id].push_back(std::unique_ptr<Component>(component));
    for(auto itr = subscribers[component->type].begin(); itr != subscribers[component->type].end(); ++itr)
    {
        (*itr)->entitiesWithComponent[component->type].insert(id);
    }
    return true;
}

bool EntityManager::addComponents(entityID id, const std::initializer_list<Component*> &components)
{
    if(!entities.count(id))
        return false;
    for(auto itr = components.begin(); itr != components.end(); ++itr)
        addComponent(id,*itr);
    return true;
}

Component* EntityManager::getComponent(entityID id, const std::string &componentType)
{
    if(!entities.count(id))
        return nullptr;
    std::vector<std::unique_ptr<Component>>* components = &(entities[id]);
    for(unsigned int i = 0; i < components->size(); ++i)
    {
        if((*components)[i]->type == componentType)
            return (*components)[i].get();
    }
    return nullptr;
}

std::vector<entityID> EntityManager::getEntitiesWithComponent(const std::string &componentType)
{
    std::vector<entityID> output;
    for(auto itr = entities.begin(); itr != entities.end(); ++itr)
    {
        if(getComponent(itr->first, componentType) != nullptr)
            output.push_back(itr->first);
    }
    return output;
}

std::vector<Component*> EntityManager::getComponentsOfType(const std::string &componentType)
{
    std::vector<Component*> output;
    for(auto itr = entities.begin(); itr != entities.end(); ++itr)
    {
        Component* current = getComponent(itr->first, componentType);
        if(current != nullptr)
            output.push_back(current);
    }
    return output;
}

void EntityManager::addSubscriber(const std::string &componentType, System* subscriber)
{
    subscribers[componentType].insert(subscriber);
}