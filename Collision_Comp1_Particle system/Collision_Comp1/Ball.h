#ifndef BALL_H
#define BALL_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h> 

#include "shaderClass.h"

class Ball
{
	public:
		Ball(float radius, int sectorCount, int stackCount, glm::vec3 color); 
		// Size of the ball, sectorCount number of segments (determen how smooth the sphere looks), 
		// stackCount number of vertical division from top to bottom (determen how smooth the sphere looks)

		void DrawBall(Shader &shader);
		~Ball();

		void UpdatePosition(float deltaTime);
	
		glm::vec3 position; // Add position variable to keep track of the ball's current position

	private:
		void generateBall();
		// Is used to generate the geometry of the ball.
		// Calculates the balls vertices and indices.

		float radius;
		int sectorCount;
		int stackCount;
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		glm::vec3 color;
		

		GLuint VAO, VBO, EBO;
};
#endif // !BALL_H


