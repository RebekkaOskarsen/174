#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <vector>
#include "Components.h"
#include "Collision.h"

class MovementSystem 
{
public:
    void update(std::vector<PositionComponent>& positions, std::vector<VelocityComponent>& velocities, float deltaTime, float speedFactor)
    {
        for (size_t i = 0; i < positions.size(); ++i) 
        {
            positions[i].position += velocities[i].velocity * deltaTime * speedFactor;
        }
    }
};

class CollisionSystem 
{
public:
    void resolveWallCollisions(std::vector<PositionComponent>& positions, std::vector<VelocityComponent>& velocities, float minX, float maxX, float minY, float maxY, float radius) 
    {
        for (size_t i = 0; i < positions.size(); ++i) 
        {
            Collision::checkWallCollision(positions[i].position, velocities[i].velocity, minX, maxX, minY, maxY, radius);
        }
    }

    void resolveBallCollisions(std::vector<PositionComponent>& positions, std::vector<VelocityComponent>& velocities, float radius) 
    {
        for (size_t i = 0; i < positions.size(); ++i) 
        {
            for (size_t j = i + 1; j < positions.size(); ++j) 
            {
                Collision::responseBallCollision(positions[i].position, positions[j].position, velocities[i].velocity, velocities[j].velocity, radius);
            }
        }
    }
};
#endif // SYSTEMS_H