#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Window.h"

float toRadians(float deg);
void createGrid();

const int WIDTH = 1024, HEIGHT = 768;
std::vector<Mesh*> meshList;
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 0.05f, 1.0f); // Initialize camera
Window window;

int main(int argc, char* argv[])
{
	window = Window(WIDTH, HEIGHT);
	window.initialise();

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Creating grid
	createGrid();
	Shader gridShader = Shader("shader.vs", "shader.fs");

	glm::mat4 projection(1.0f);
	projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);


	while (!window.getShouldClose())
	{

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Camera movement
		camera.pan(window.getKeys(), window.getDeltaX());
		camera.tilt(window.getKeys(), window.getDeltaY());

		gridShader.use();

		// Model matrix
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -1.0f));
		model = glm::rotate(model, toRadians(45), glm::vec3(1.0f, 0.0f, 0.0f));

		// View matrix
		glm::mat4 view(1.0f);
		view = glm::rotate(view, toRadians(-85), glm::vec3(1.0f, 0.0f, 0.0f));
		view = camera.calculateViewMatrix() * view;

		// Connect matrices with shaders
		gridShader.setMatrix4Float("model", &model);
		gridShader.setMatrix4Float("projection", &projection);
		gridShader.setMatrix4Float("view", &view);
		meshList[0]->RenderMesh(GL_LINES);

		gridShader.free();

		//check and call events and sawp buffers
		window.swapBuffers();
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

float toRadians(float deg)
{
	return deg * (3.14159265f / 180.0f);
}

void createGrid()
{
	int squareCount = 128;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i <= squareCount; i++) {
		for (int j = 0; j <= squareCount; j++) {
			float x = (float)j / (float)squareCount;
			float y = 0;
			float z = (float)i / (float)squareCount;

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	for (int i = 0; i < squareCount; i++) {
		for (int j = 0; j < squareCount; j++) {
			int top = i * (1 + squareCount);
			int bottom = (i + 1) * (1 + squareCount);

			// Top line
			indices.push_back(top + j);
			indices.push_back(top + j + 1);
			// Right line
			indices.push_back(top + j + 1);
			indices.push_back(bottom + j + 1);
			// Bottom line
			indices.push_back(bottom + j + 1);
			indices.push_back(bottom + j);
			// Left line
			indices.push_back(bottom + j);
			indices.push_back(top + j);
		}
	}
	Mesh* gridObj = new Mesh();
	gridObj->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(gridObj);
}