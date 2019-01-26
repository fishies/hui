#pragma once

//#include <assert.h>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>
#include <initializer_list>
#include <memory>

using entityID = unsigned int;
#define entityId entityID

struct Component;
class System;

class EntityManager
{
    std::unordered_map<entityID,std::vector<std::unique_ptr<Component>>> entities;//change to priority heap MRU?
    std::queue<entityID> idPool;
    entityID highestID;
    std::unordered_map<std::string,std::unordered_set<System*>> subscribers;

public:
    EntityManager();

    entityID addEntity(const std::initializer_list<Component*> &components);
    bool removeEntity(entityID id);
    bool addComponent(entityID id, Component* component);
    bool addComponents(entityID id, const std::initializer_list<Component*> &components);
    Component* getComponent(entityID id, const std::string &componentType);
    std::vector<entityID> getEntitiesWithComponent(const std::string &componentType);
    std::vector<Component*> getComponentsOfType(const std::string &componentType);
    void addSubscriber(const std::string &componentType, System* subscriber);

    //overloaded [] to access vector<component> of specific entity?
};

struct Component
{
    std::string type;
    Component(const std::string &name = "Component");
};

class System
{
protected:
    EntityManager* entityManager;
public:
    std::unordered_map<std::string,std::unordered_set<entityID>> entitiesWithComponent;

    System(EntityManager* entityAdmin);
    virtual void tick() = 0;
    void subscribe(const std::string &componentType);
};
