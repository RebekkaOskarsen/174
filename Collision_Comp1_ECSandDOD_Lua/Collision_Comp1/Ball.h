#ifndef BALL_H
#define BALL_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include "shaderClass.h"

class Ball 
{
public:
    Ball(float radius, glm::vec3 color);
    ~Ball();

    void draw(Shader& shader, const glm::vec3& position);

private:
    float radius;

    glm::vec3 color;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void generateBall();

    unsigned int VAO, VBO, EBO;
};
#endif // !BALL_H


