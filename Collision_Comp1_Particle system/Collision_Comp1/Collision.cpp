#include "Collision.h"

bool Collision::checkBallCollision(const glm::vec3& pos1, const glm::vec3& pos2, float radius) 
{
    float distance = glm::length(pos1 - pos2); // This calculates the distance between the two balls 
    return distance <= 1.90f * radius; // Checks if the distance between the two balls is less than or equal to 1.90*radius
}
// pos1 is the position of the first ball
// pos2 is the position of the second ball
// radius is for each ball
// This boolian calculates the distance between the centers of two balls.

void Collision::checkWallCollision(glm::vec3& position, glm::vec3& velocity, float minX, float maxX, float minY, float maxY, float radius) 
// minX and maxX values for the x-axis boundary, left and right walls
// minY and maxY values for the y-axis boundary, top and bottom walls
// radius is the radius of the ball, to make sure that the balls surface does not pass through the walls.
{
    if (position.x + radius > maxX || position.x - radius < minX) 
    {
        velocity.x = -velocity.x; // Reverse x velocity, causing the ball to bounce off the wall
        position.x = glm::clamp(position.x, minX + radius, maxX - radius); // Clamp position within bounds
    }
    if (position.y + radius > maxY || position.y - radius < minY) 
    {
        velocity.y = -velocity.y; // Reverse y velocity, causing the ball to bounce off the wall
        position.y = glm::clamp(position.y, minY + radius, maxY - radius); // Clamp position within bounds
    }
}
// Clamping is to make sure that the ball does not try to go outside
// the walls and restricts the position between minX + radius and maxX-radius.
// That the ball does not go out of bounds.

void Collision::responseBallCollision(glm::vec3& pos1, glm::vec3& pos2, glm::vec3& vel1, glm::vec3& vel2, float radius)
{
    if (checkBallCollision(pos1, pos2, radius))// Checks if the two balls have collided
    {
        // Separate the balls by moving them apart
        glm::vec3 direction = glm::normalize(pos1 - pos2);
        float overlap = (1.90f * radius) - glm::length(pos1 - pos2);

        pos1 += direction * (overlap / 2.0f);  // Move ball 1
        pos2 -= direction * (overlap / 2.0f);  // Move ball 2
        // moving both ball by half the overlap distance, so that the collision is resolved equaly.
        
        // Reverse velocities once the balls are separated.
        vel1 = -vel1;
        vel2 = -vel2;
        // Simulates the effect of bouncing off eachother.
    }
}
// vel1 and vel2 is the velocity of the two balls.
// Direction: calculates the normalized direction vector from ball 2 to ball 1.
// Overlap: calculates how much the two balls are overlapping.
