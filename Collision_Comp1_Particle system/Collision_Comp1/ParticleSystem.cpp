#include "ParticleSystem.h"

void ParticleSystem::initParticle(Particle& particle, const glm::vec3& emitterPosition)
{
    float RainRadius = 10.0f; // Radius of the rain area

    // Randomize particle's starting position within the rain radius
    particle.position = emitterPosition + glm::vec3(
        static_cast<float>(rand() % 200 - 100) / 100.0f * RainRadius, // x-axis randomness
        static_cast<float>(rand() % 30) / 10.0f, // Random initial height
        static_cast<float>(rand() % 200 - 100) / 100.0f * RainRadius  // z-axis randomness
    );

    particle.velocity = glm::vec3(0.0f, -4.0f, 0.0f);

    // Random lifetime
    particle.life = static_cast<float>(rand() % 100) / 100.0f; // Randomized lifetime
}

ParticleSystem::ParticleSystem(unsigned int maxParticles, const glm::vec3& emitterPosition)
    : maxParticles(maxParticles)
{
    particles.resize(maxParticles);
    for (auto& particle : particles)
    {
        initParticle(particle, emitterPosition);
    }

    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 2 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void ParticleSystem::Update(float deltaTime, const glm::vec3& emitterPosition)
{
    for (auto& particle : particles)
    {
        particle.position += particle.velocity * deltaTime; // Update position based on velocity

        // Reset particle when it reaches the ground (y = -3.0f)
        if (particle.position.y <= -3.0f)
        {
            initParticle(particle, emitterPosition);
        }
    }
}

void ParticleSystem::DrawRain(Shader& shader)
{
    std::vector<glm::vec3> vertices;
    for (const auto& particle : particles)
    {
        if (particle.life > 0.0f)
        {
            vertices.push_back(particle.position); // Start of the line
            vertices.push_back(particle.position + glm::vec3(0.0f, -0.1f, 0.0f)); // End of the line
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Render
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, static_cast<int>(vertices.size()));
    glBindVertexArray(0);
}


