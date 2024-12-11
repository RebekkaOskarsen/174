#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>

class EntityManager {
public:
    using Entity = size_t;

    Entity createEntity() 
    {
        entities.push_back(nextEntityId);
        return nextEntityId++;
    }

    const std::vector<Entity>& getEntities() const 
    {
        return entities;
    }

private:
    std::vector<Entity> entities;
    Entity nextEntityId = 0;
};

#endif // ENTITY_MANAGER_H
