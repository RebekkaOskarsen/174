#include "Collision.h"

bool Collision::checkBallCollision(const glm::vec3& pos1, const glm::vec3& pos2, float radius) {
    float distance = glm::length(pos1 - pos2);
    return distance <= 1.9f * radius; // Checks if the balls are colliding
}

void Collision::checkWallCollision(glm::vec3& position, glm::vec3& velocity, float minX, float maxX, float minY, float maxY, float radius) {
    if (position.x + radius > maxX || position.x - radius < minX) {
        velocity.x = -velocity.x;
        position.x = glm::clamp(position.x, minX + radius, maxX - radius);
    }
    if (position.y + radius > maxY || position.y - radius < minY) {
        velocity.y = -velocity.y;
        position.y = glm::clamp(position.y, minY + radius, maxY - radius);
    }
}

void Collision::responseBallCollision(glm::vec3& pos1, glm::vec3& pos2, glm::vec3& vel1, glm::vec3& vel2, float radius) {
    if (checkBallCollision(pos1, pos2, radius)) {
        glm::vec3 direction = glm::normalize(pos1 - pos2);
        float overlap = (1.9f * radius) - glm::length(pos1 - pos2);

        pos1 += direction * (overlap / 2.0f);
        pos2 -= direction * (overlap / 2.0f);

        vel1 = -vel1;
        vel2 = -vel2;
    }
}