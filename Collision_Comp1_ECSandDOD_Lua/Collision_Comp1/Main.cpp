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

extern "C"
{
#include "Lua/include/lua.h"
#include "Lua/include/lauxlib.h"
#include "Lua/include/lualib.h"
}

// Link to lua library
#ifdef _WIN32
#pragma comment(lib, "Lua/lua54.lib")
#endif

#include "EntityManager.h"
#include "Components.h"
#include "Systems.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int SCR_WIDTH, int SCR_HEIGHT); 
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

// ECS
EntityManager entityManager;
std::vector<PositionComponent> positions;
std::vector<VelocityComponent> velocities;
std::vector<BallComponent> balls;

lua_State* L; //This is lua

int lua_createEntity(lua_State* L)
{
	cout << "AddEntity" << endl;
	auto entity = entityManager.createEntity();

	balls.push_back({ ballRadius, glm::vec3(1) });
	return 0;
}

int lua_addPositionComponent(lua_State* L) 
{
	cout << "AddPos" << endl;
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);
	float z = luaL_checknumber(L, 3);

	positions.push_back({ glm::vec3(x, y, z) });
	return 0;
}

int lua_addVelocityComponent(lua_State* L) 
{
	cout << "AddVel" << endl;
	float vx = luaL_checknumber(L, 1);
	float vy = luaL_checknumber(L, 2);
	float vz = luaL_checknumber(L, 3);

	velocities.push_back({ glm::vec3(vx, vy, vz) });
	return 0;
}

void registerLuaBindings(lua_State* L) 
{
	lua_register(L, "createEntity", lua_createEntity);
	lua_register(L, "addPositionComponent", lua_addPositionComponent);
	lua_register(L, "addVelocityComponent", lua_addVelocityComponent);
}

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
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse

	//Initialize GLAD
	gladLoadGL();

	//Set the viewport
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	Shader shaderProgram("default.vert", "default.frag");

	Box box;

	//Ball
	Ball ballTemplate(ballRadius, glm::vec3(0.8f, 0.0f, 0.0f));

	//Systems
	MovementSystem movementSystem;
	CollisionSystem collisionSystem;

	//Particle: Rain
	glm::vec3 rainEmitterPosition(1.0f, 5.0f, 0.0f); // Position where rain starts
	ParticleSystem rainSystem(1000, rainEmitterPosition);

	//Lua
	L = luaL_newstate();// initialize Lua interpreter
	registerLuaBindings(L);
	luaL_openlibs(L);// load Lua base libraries (print / math / etc)
	luaL_dofile(L, "C:/Spillmotorarkitektur/Entities.txt");
	

	glEnable(GL_DEPTH_TEST);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		
		
		// Update ECS systems
		movementSystem.update(positions, velocities, deltaTime, speedFactor);
		collisionSystem.resolveWallCollisions(positions, velocities, -2.0f, 2.0f, 0.0f, 4.0f, ballRadius);
		collisionSystem.resolveBallCollisions(positions, velocities, ballRadius);

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
		for (size_t i = 0; i < positions.size(); ++i) 
		{
			ballTemplate.draw(shaderProgram, positions[i].position);
		}

		
		shaderProgram.setMat4("model", glm::mat4(1.0f));

		// Draw Rain
		rainSystem.Update(deltaTime, rainEmitterPosition);
		rainSystem.DrawRain(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	lua_close(L);
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
		speedFactor += 2.0f; // Increase speed
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		speedFactor = max(2.0f, speedFactor - 2.0f);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		luaL_dofile(L, "C:/Spillmotorarkitektur/Entities.txt");
	}
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