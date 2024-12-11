#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp>

// Component to store position
struct PositionComponent {
    glm::vec3 position;
};

// Component to store velocity
struct VelocityComponent {
    glm::vec3 velocity;
};

// Component for ball-specific properties
struct BallComponent {
    float radius;
    glm::vec3 color;
};
#endif // COMPONENTS_H
