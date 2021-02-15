#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <glad/glad.h>	
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "shader.h"
#include "Grid.h"
#include "Mesh.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
float toRadians(float deg);
void createGrid();

const int WIDTH = 800, HEIGHT = 600;
std::vector<Mesh*> meshList;

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intiliaze GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	
	// Creating grid
	createGrid();
	Shader gridShader = Shader("shader.vs", "shader.fs");

	glm::mat4 projection(1.0f);
	projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);


	while (!glfwWindowShouldClose(window))
	{
		// manage input
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gridShader.use();

		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -1.0f));
		model = glm::rotate(model, toRadians(45), glm::vec3(1.0f, 0.0f, 0.0f));
		gridShader.setMatrix4Float("model", &model);
		gridShader.setMatrix4Float("projection", &projection);
		meshList[0]->renderMesh(GL_LINES);

		gridShader.free();

		//check and call events and sawp buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

float toRadians(float deg)
{
	return deg * (3.14159265f / 180.0f);
}

void createGrid()
{
	Grid* grid = new Grid(128);
	
	std::vector<float> vertices = grid->getVertexArr();
	std::vector<unsigned int> indices = grid->getIndexArr();
	unsigned int indexCount = grid->getIndexCount();

	Mesh* gridObj = new Mesh();
	gridObj->createMesh(&vertices[0], &indices[0], indices.size(), vertices.size());
	meshList.push_back(gridObj);
}