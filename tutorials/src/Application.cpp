#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "Shader.h"
#include "Texture.h"


enum BufferType
{
	VERTS = 0, ELEMENTS = 1
};

#define SCRN_WIDTH 640
#define SCRN_HEIGHT 480

void CreateRectangle(unsigned int& VAO, const float* triangle_coords)
{
	/*
	0	|   1
	---------
	2	|	3
	*/

	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	//This wrapps the all of the next opengl commands and saves them 
	//inside the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create a buffer and get the buffer id
	//so you will know how to access your newly created buffer
	unsigned int  buffer[2];
	glGenBuffers(2, buffer);


	//State which buffer we are about to manipulate
	//State the attributes of the buffer like its size and its data 
	glBindBuffer(GL_ARRAY_BUFFER, buffer[BufferType::VERTS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, triangle_coords, GL_STATIC_DRAW);

	//Add the element array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[BufferType::ELEMENTS]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Enable the attribute first!! in order to let the shaders know which buffer corresponds to which variable
	glEnableVertexAttribArray(0); // the attribute is located at the start of the data
	//Describe the data from the coords array to opengl
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


	//Unbind the vertex array. No late confusion
	glBindVertexArray(0);
}


void CreateRectangleTexture(unsigned int& VAO, const float* paintData)
{
	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	unsigned int buffers[2];
	glGenBuffers(2, buffers);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[BufferType::VERTS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8*2, paintData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[BufferType::ELEMENTS]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	// Texture Pos
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *) (2 * sizeof(float)));

}

void glmFun()
{
	glm::mat2 my_mat = glm::mat2(1.0f);
	
	std::cout << glm::to_string(my_mat) << std::endl;
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "Hello GL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	GLFWmonitor *primary = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(primary);
	glfwSetWindowPos(window, (mode->width - SCRN_WIDTH) / 2, (mode->height - SCRN_HEIGHT) / 2);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	unsigned int VAO1;

	const float triangle_coords[] = {
		-0.5f, 0.5f,
		 0.5f, 0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f,
	};

	CreateRectangle(VAO1, triangle_coords);
	//Tell opengl how to add the geometry and the color (shading)
	Shader m_Shader("res/shaders/Basic.shader");


	unsigned int VAO2;
	const float rectangle_texture[] = {
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f
	};
	CreateRectangleTexture(VAO2, rectangle_texture);
	Texture m_Texture("res/images/brick.jpg");
	Shader tex_Shader("res/shaders/Texture.shader");
	
	glmFun();

	unsigned int VAO3;
	const float rectangle_texture2[] = {
		-0.5f, 0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 0.0f
	};
	CreateRectangleTexture(VAO3, rectangle_texture2);
	Texture glm_Texture("res/images/brick.jpg");
	Shader glm_Shader("res/shaders/Glm.shader");
	
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//glClearColor(1.0, 0.5, 0.0, 1.0); // paint the viewport

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		

		glViewport((int)(640 / 2), (int)(480 / 2), (int)(640 / 2), (int)(480 / 2));
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id[BufferType::ELEMENTS]);
		m_Shader.Use();
		m_Shader.SetColor(1.0f, 0.5f, 0.0f, 1.0f);
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		glViewport(0, 0, (int)(640 / 2), (int)(480 / 2));
		m_Texture.Use();
		tex_Shader.Use();
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glViewport(0, (int)(480 / 2), (int)(640 / 2), (int)(480 / 2));
		glm_Texture.Use();
		glm_Shader.Use();

		glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::translate(trans, glm::vec3(1.0f, 0.0f, 0.0f));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		std::cout << glfwGetTime() << std::endl;

		glm_Shader.setUniformMatrix4fv("trans", glm::value_ptr(trans));
		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

