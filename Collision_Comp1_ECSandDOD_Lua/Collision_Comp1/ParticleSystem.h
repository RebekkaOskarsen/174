#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <glm/glm.hpp>
#include <vector>

#include "shaderClass.h"

struct Particle 
{
    glm::vec3 position;
    glm::vec3 velocity;
    float life;

    Particle() : position(0.0f), velocity(0.0f), life(0.0f) {}
};

class ParticleSystem
{
public:
    ParticleSystem(unsigned int maxParticles, const glm::vec3& emitterPosition);
    ~ParticleSystem();

    void Update(float deltaTime, const glm::vec3& emitterPosition);
    void DrawRain(Shader& shader);

private:
    std::vector<Particle> particles;
    unsigned int maxParticles;
    unsigned int VAO, VBO;

    void initParticle(Particle& particle, const glm::vec3& emitterPosition);

};
#endif // !PARTICLESYSTEM_H



