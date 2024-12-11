#include "Box.h"

Box::Box()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;

	GLfloat boxvertices[] =
	{
		//position				//Color
		//Bottom wall
		-2.0f, 0.0f, 2.0f,		1.0f, 0.0f, 0.0f,
		 2.0f, 0.0f, 2.0f,		1.0f, 0.0f, 0.0f,
		 2.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		-2.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,

		//Back wall
		-2.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,
		 2.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,
		 2.0f, 4.0f, 0.0f,		1.0f, 1.0f, 1.0f,
		-2.0f, 4.0f, 0.0f,		1.0f, 1.0f, 1.0f,

		//Left wall
		-2.0f, 0.0f, 2.0f,		0.0f, 0.0f, 1.0f,
		-2.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		-2.0f, 4.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		-2.0f, 4.0f, 2.0f,		0.0f, 0.0f, 1.0f,

		//Right wall
		 2.0f, 0.0f, 2.0f,		0.0f, 1.0f, 0.0f,
		 2.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		 2.0f, 4.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		 2.0f, 4.0f, 2.0f,		0.0f, 1.0f, 0.0f,

		 //Top wall
		-2.0f, 4.0f, 2.0f,		1.0f, 0.0f, 1.0f,
		 2.0f, 4.0f, 2.0f,		1.0f, 0.0f, 1.0f,
		 2.0f, 4.0f, 0.0f,		1.0f, 0.0f, 1.0f,
		-2.0f, 4.0f, 0.0f,		1.0f, 0.0f, 1.0f,

	};

	GLuint boxindices[] =
	{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(boxvertices), boxvertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxindices), boxindices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


}

Box::~Box()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Box::DrawBox()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
