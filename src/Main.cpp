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
#include "IndependentMesh.h"
#include "ComplexObject.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
float toRadians(float deg);
void createGrid();

// Character creation methods

/// <summary>
/// Creates characters representing the first and last letter of each team member's name, and the first and last digit of their student ID.
/// This then adds those created objects to the object list.
/// </summary>
/// <param name="shader">The shader that will be used to render the objects</param>
void CreateLetters(Shader* shader);
/// <summary>
/// Creates the letter J using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the letter J</returns>
ComplexObject* CreateLetterJ(GLuint uniformModel);
/// <summary>
/// Creates the letter L using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the letter L</returns>
ComplexObject* CreateLetterL(GLuint uniformModel);


// Global Variables

const int WIDTH = 1024, HEIGHT = 768;
std::vector<Mesh*> meshList;
std::vector<ComplexObject*> objectList;
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 0.05f, 1.0f); // Initialize camera
Window window;
const float BASE_WORLD_ANGLE = -5.0f;
const float BASE_WORLD_Z_POS = 1.0f;
const float BASE_WORLD_Y_POS = -0.5f;
const float BASE_WORLD_X_POS = 0.0f;
float currentWorldAngle = BASE_WORLD_ANGLE;
float currentXPos = BASE_WORLD_X_POS;
float currentYPos = BASE_WORLD_Y_POS;
float currentZpos = BASE_WORLD_Z_POS;
float worldRotationIncrement = 0.5f;
float worldPosIncrement = 0.01f;


int main(int argc, char* argv[])
{
	// Initializing Global Variables
	meshList = std::vector<Mesh*>();
	objectList = std::vector<ComplexObject*>();

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

	// Creating the letters
	CreateLetters(&gridShader);

	

	while (!window.getShouldClose())
	{

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Camera movement
		camera.pan(window.getKeys(), window.getDeltaX());
		camera.tilt(window.getKeys(), window.getDeltaY());

		gridShader.use();

		// Handling rotations
		// Rotating the entire world dependent on key presses.
			// We rotate around the X-Axis
		if (window.getKeys()[GLFW_KEY_RIGHT])
		{
			// Anticlockwise rotation
			currentWorldAngle += worldRotationIncrement;
		}
		if (window.getKeys()[GLFW_KEY_LEFT])
		{
			// Clockwise rotation
			currentWorldAngle -= worldRotationIncrement;
		}
		if (window.getKeys()[GLFW_KEY_HOME])
		{
			// Reset to default rotation.
			currentWorldAngle = BASE_WORLD_ANGLE;
			currentZpos = BASE_WORLD_Z_POS;
		}
		if (window.getKeys()[GLFW_KEY_UP])
		{
			currentZpos -= worldPosIncrement;
		}
		if (window.getKeys()[GLFW_KEY_DOWN])
		{
			currentZpos += worldPosIncrement;
		}
		if (window.getKeys()[GLFW_KEY_W])
		{
			currentYPos -= worldPosIncrement;
		}
		if (window.getKeys()[GLFW_KEY_S])
		{
			currentYPos += worldPosIncrement;
		}
		if (window.getKeys()[GLFW_KEY_A])
		{
			currentXPos -= worldPosIncrement;
		}
		if (window.getKeys()[GLFW_KEY_D])
		{
			currentXPos += worldPosIncrement;
		}
		if (window.getKeys()[GLFW_KEY_F])
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (window.getKeys()[GLFW_KEY_L])
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}


		// Model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, toRadians(0), glm::vec3(1.0f, 0.0f, 0.0f));


		// View matrix
		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(currentXPos, currentYPos, currentZpos));
		view = glm::rotate(view, toRadians(currentWorldAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		view = camera.calculateViewMatrix() * view;


		

		// Connect matrices with shaders
		gridShader.setMatrix4Float("model", &model);
		gridShader.setMatrix4Float("projection", &projection);
		gridShader.setMatrix4Float("view", &view);
		meshList[0]->RenderMesh(GL_LINES);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.0f));
		objectList[0]->SetModelMatrix(model, 0);
		objectList[0]->RenderObject();

		gridShader.free();

		//check and call events and swap buffers
		window.swapBuffers();
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

void CreateLetters(Shader* shader) {
	GLuint modelLocation = shader->getLocation("model");

	// Creating Joel's name and ID object

	ComplexObject* letterJ = CreateLetterJ(modelLocation);
	ComplexObject* letterL = CreateLetterL(modelLocation);

	// Making the translations so that both letters can be side by side.

	glm::mat4 model = glm::mat4(1.0f);

	// J translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	letterJ->SetModelMatrix(model, modelLocation);

	// L translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	letterL->SetModelMatrix(model, modelLocation);

	ComplexObject* joelNameAndID = new ComplexObject();
	joelNameAndID->objectList.push_back(letterJ);
	joelNameAndID->objectList.push_back(letterL);

	objectList.push_back(joelNameAndID);
}

ComplexObject* CreateLetterJ(GLuint uniformModel)
{
	// First Cube
   // The center of the screen is 0.0f. It also goes from -1.0 to 1.0.
   // x, y, z
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.5f, // bottom top left corner, index 0
		-0.5f, -0.5f, -0.5f, // bottom bottom left corner, index 1
		0.5f, -0.5f, -0.5f, // bottom bottom right corner, index 2
		0.5f, -0.5f, 0.5f, // bottom top right corner, index 3
		-0.5f, 0.5f, 0.5f, // top top left corner, index 4
		-0.5f, 0.5f, -0.5f, // top bottom left corner, index 5
		0.5f, 0.5f, -0.5f, // top bottom right corner, index 6
		0.5f, 0.5f, 0.5f, // top top right corner, index 7
	};

	// Creating the order of draws for indexed draws.
	// This uses the position of the vertices in vertices[]. By specifying them in order, we define the drawing order of those points.
	// So 0,3,1 means draw point at index 0, then at index 3, then at index 1.
	// Note we are specifying them in counter clockwise order. This is to say to opengl that we are drawing front faces.
	// We have to draw triangles, because they are the easiest polygon. So each group of 3 ints is a triangle.
	// We are drawing top triangle, then bottom triangle, with the top triangle having its middle in the top left corner of a face.
	unsigned int indices[] = {
		// Left face
		0, 5, 4,    0, 1, 5,
		// Front face
		1, 6, 5,    1, 2, 6,
		// Right Face
		2, 7, 6,    2, 3, 7,
		// Back face
		3, 4, 7,    3, 0, 4,
		// Top
		5, 7, 4,    5, 6, 7,
		// Bottom
		1, 3, 0,    1, 2, 3
	};

	// LETTER J

	// Creating the base of the letter J
	IndependentMesh* jBase = new IndependentMesh();
	jBase->CreateMesh(vertices, indices, 24, 36);
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));

	jBase->SetModelMatrix(model, uniformModel);

	// Creating the trunk of the letter J
	IndependentMesh* jTrunk = new IndependentMesh();
	jTrunk->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));

	jTrunk->SetModelMatrix(model, uniformModel);

	// Creating the paw of the letter J
	IndependentMesh* jPaw = new IndependentMesh();
	jPaw->CreateMesh(vertices, indices, 24, 36);

	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(-2.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	jPaw->SetModelMatrix(model, uniformModel);

	ComplexObject* jLetter = new ComplexObject();
	jLetter->meshList.push_back(jBase);
	jLetter->meshList.push_back(jTrunk);
	jLetter->meshList.push_back(jPaw);

	return jLetter;
}

ComplexObject* CreateLetterL(GLuint uniformModel)
{
	// First Cube
   // The center of the screen is 0.0f. It also goes from -1.0 to 1.0.
   // x, y, z
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.5f, // bottom top left corner, index 0
		-0.5f, -0.5f, -0.5f, // bottom bottom left corner, index 1
		0.5f, -0.5f, -0.5f, // bottom bottom right corner, index 2
		0.5f, -0.5f, 0.5f, // bottom top right corner, index 3
		-0.5f, 0.5f, 0.5f, // top top left corner, index 4
		-0.5f, 0.5f, -0.5f, // top bottom left corner, index 5
		0.5f, 0.5f, -0.5f, // top bottom right corner, index 6
		0.5f, 0.5f, 0.5f, // top top right corner, index 7
	};

	// Creating the order of draws for indexed draws.
	// This uses the position of the vertices in vertices[]. By specifying them in order, we define the drawing order of those points.
	// So 0,3,1 means draw point at index 0, then at index 3, then at index 1.
	// Note we are specifying them in counter clockwise order. This is to say to opengl that we are drawing front faces.
	// We have to draw triangles, because they are the easiest polygon. So each group of 3 ints is a triangle.
	// We are drawing top triangle, then bottom triangle, with the top triangle having its middle in the top left corner of a face.
	unsigned int indices[] = {
		// Left face
		0, 5, 4,    0, 1, 5,
		// Front face
		1, 6, 5,    1, 2, 6,
		// Right Face
		2, 7, 6,    2, 3, 7,
		// Back face
		3, 4, 7,    3, 0, 4,
		// Top
		5, 7, 4,    5, 6, 7,
		// Bottom
		1, 3, 0,    1, 2, 3
	};

	// LETTER L
	glm::mat4 model = glm::mat4(1.0f);

	 // Creating the base of the letter L
	IndependentMesh* lBase = new IndependentMesh();
	lBase->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));

	lBase->SetModelMatrix(model, uniformModel);

	// Creating the trunk of the letter L
	IndependentMesh* lTrunk = new IndependentMesh();
	lTrunk->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));

	lTrunk->SetModelMatrix(model, uniformModel);

	ComplexObject* lLetter = new ComplexObject();
	lLetter->meshList.push_back(lBase);
	lLetter->meshList.push_back(lTrunk);

	return lLetter;
}
