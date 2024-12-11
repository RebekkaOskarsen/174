#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "Camera.h"
#include "Box.h"
#include "Ball.h"
#include "Collision.h"
#include "ParticleSystem.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int SCR_WIDTH, int SCR_HEIGHT); 
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window); 

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;
float speedFactor = 0.0f; // Default speed factor
float ballRadius = 0.2; // Radius of the balls

glm::vec3 ballVelocities[] = 
{
	glm::vec3(0.01f, 0.01f, 0.0f), // velocity for ball1
	glm::vec3(-0.01f, 0.01f, 0.0f), // velocity for ball2
	glm::vec3(0.01f, -0.01f, 0.0f), // velocity for ball3
	glm::vec3(-0.02f, 0.01f, 0.0f), // velocity for ball4
	glm::vec3(0.01f, -0.02f, 0.0f), // velocity for ball5
	glm::vec3(-0.01f, -0.01f, 0.0f), // velocity for ball6
	glm::vec3(0.01f, 0.02f, 0.0f), // velocity for ball7
	glm::vec3(-0.02f, 0.01f, 0.0f), // velocity for ball8
	glm::vec3(0.01f, -0.02f, 0.0f), // velocity for ball9
	glm::vec3(-0.01f, 0.01f, 0.0f), // velocity for ball10
	glm::vec3(0.01f, -0.01f, 0.0f), // velocity for ball11
	glm::vec3(-0.01f, 0.02f, 0.0f)  // velocity for ball12
};

glm::vec3 ballPositions[] = 
{
	glm::vec3(0.0f, 0.5f, 1.0f), // initial position for ball1
	glm::vec3(1.5f, 0.5f, 1.0f), // initial position for ball2
	glm::vec3(-1.5f, 0.5f, 1.0f), // initial position for ball3
	glm::vec3(0.0f, 1.5f, 1.0f), // initial position for ball4
	glm::vec3(1.5f, 1.5f, 1.0f), // initial position for ball5
	glm::vec3(-1.5f, 1.5f, 1.0f), // initial position for ball6
	glm::vec3(0.0f, 2.5f, 1.0f), // initial position for ball7
	glm::vec3(1.5f, 2.5f, 1.0f), // initial position for ball8
	glm::vec3(-1.5f, 2.5f, 1.0f), // initial position for ball9
	glm::vec3(0.0f, 3.5f, 1.0f), // initial position for ball10
	glm::vec3(1.5f, 3.5f, 1.0f), // initial position for ball11
	glm::vec3(-1.5f, 3.5f, 1.0f)  // initial position for ball12
};


int main()
{
	glfwInit(); //Initialize GLFW

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Oblig 1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse

	//Initialize GLAD
	gladLoadGL();

	//Set the viewport
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	Shader shaderProgram("default.vert", "default.frag");

	Box box;

	//Balls: 12
	Ball ball1(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball2(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball3(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball4(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball5(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball6(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball7(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball8(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball9(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball10(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball11(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 
	Ball ball12(ballRadius, 36, 18, glm::vec3(0.8f, 0.0f, 0.0f)); 

	//Particle: Rain
	glm::vec3 rainEmitterPosition(1.0f, 5.0f, 0.0f); // Position where rain starts
	ParticleSystem rainSystem(1000, rainEmitterPosition);

	glEnable(GL_DEPTH_TEST);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) // Check if the window should close
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		
		for (int i = 0; i < 12; ++i) 
		{
			ballPositions[i] += ballVelocities[i] * deltaTime * speedFactor; // Ball moves

			// Check for wall collisions
			Collision::checkWallCollision(ballPositions[i], ballVelocities[i], -2.0f, 2.0f, 0.0f, 4.0f, ballRadius);

			// Check for collisions with other balls and resolve them
			for (int j = i + 1; j < 12; ++j)
			{
				Collision::responseBallCollision(ballPositions[i], ballPositions[j], ballVelocities[i], ballVelocities[j], ballRadius);
			}
		}

		//Render
		glClearColor(0.5f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); // perspective projection matrix
		shaderProgram.setMat4("projection", projection); // pass the projection matrix to the shader

		glm::mat4 view = camera.GetViewMatrix();
		shaderProgram.setMat4("view", view); // pass the view matrix to the shader

		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		shaderProgram.setMat4("model", model);
		
		// Draw box
		box.DrawBox();
		
		// Draw balls
		for (int i = 0; i < 12; ++i)
		{
			// Model matrix is used to transform the object in 3D space.
			model = glm::mat4(1.0f); // Everything is set to zero.
			model = glm::translate(model, ballPositions[i]); // Moves the ball to its new position on the screen.
			shaderProgram.setMat4("model", model); // Sends the updated position to the shader program.
			switch (i)
			{
				case 0: ball1.DrawBall(shaderProgram); break;
				case 1: ball2.DrawBall(shaderProgram); break;
				case 2: ball3.DrawBall(shaderProgram); break;
				case 3: ball4.DrawBall(shaderProgram); break;
				case 4: ball5.DrawBall(shaderProgram); break;
				case 5: ball6.DrawBall(shaderProgram); break;
				case 6: ball7.DrawBall(shaderProgram); break;
				case 7: ball8.DrawBall(shaderProgram); break;
				case 8: ball9.DrawBall(shaderProgram); break;
				case 9: ball10.DrawBall(shaderProgram); break;
				case 10: ball11.DrawBall(shaderProgram); break;
				case 11: ball12.DrawBall(shaderProgram); break;
			}
			// With the switch statment i decide which ball to draw and use DrawBall() to actually render it.
		}

		shaderProgram.setMat4("model", glm::mat4(1.0f));

		rainSystem.Update(deltaTime, rainEmitterPosition);
		rainSystem.DrawRain(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// Adjust speed factor with UP and DOWN arrow keys
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		speedFactor += 3.0f; // Increase speed
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		speedFactor = max(0.1f, speedFactor - 0.1f); // Decrease speed but will not go below 0.1f.
}

void framebuffer_size_callback(GLFWwindow* window, int SCR_WIDTH, int SCR_HEIGHT)
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	std::cout << "Window resized with " << SCR_WIDTH << "Height" << SCR_HEIGHT << std::endl;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstMouse)
	{
		lastX = static_cast<float>(xpos);   // Update lastX with xpos
		lastY = static_cast<float>(ypos);   // Update lastY with ypos
		firstMouse = false;
	}

	float xoffset = static_cast<float>(xpos) - lastX;
	float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top

	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	camera.ProcessMouseMovement(xoffset, yoffset);
}