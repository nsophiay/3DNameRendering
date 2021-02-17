#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Window.h"
#include "IndependentMesh.h"
#include "ComplexObject.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
float toRadians(float deg);
/// <summary>
/// Creates a square grid by creating vertices for the given amount of squares (basically a 2d array).
/// Links all the vertices with 2-pair indices that can be used with GL_LINES to draw the triangle. 
/// </summary>
/// <param name="squareCount">Integer describing amount of squares user wishes to be created. </param>
void createGrid(int squareCount);

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

ComplexObject* CreateLetterP(GLuint uniformModel);
ComplexObject* CreateLetterR(GLuint uniformModel);
ComplexObject* CreateNumber2(GLuint uniformModel);
ComplexObject* CreateNumber4(GLuint uniformModel);

/// <summary>
/// Creates the letters SA43 using meshes and complex objects.
/// </summary>
/// <param name="shader">The shader that will be used to render the objects</param>
void drawSA43(Shader* shader);


// Global Variables

const int WIDTH = 1024, HEIGHT = 768;
std::vector<Mesh*> meshList;
std::vector<ComplexObject*> objectList;
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 0.05f, 1.0f); // Initialize camera
Window window;
const float BASE_WORLD_ANGLE = -5.0f;
const float BASE_WORLD_Y_POS = -0.5f;
float currentWorldAngle = BASE_WORLD_ANGLE;
float currentYPos = BASE_WORLD_Y_POS;
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
	createGrid(128);
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
		camera.movementFromKeyboard(window.getKeys());

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
			currentYPos = BASE_WORLD_Y_POS;
		}
		if (window.getKeys()[GLFW_KEY_UP])
		{
			currentYPos -= worldPosIncrement;
		}
		if (window.getKeys()[GLFW_KEY_DOWN])
		{
			currentYPos += worldPosIncrement;
		}
		
		if (window.getKeys()[GLFW_KEY_F])
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (window.getKeys()[GLFW_KEY_L])
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}


		// Model matrix for the world grid
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -10.0f));
		model = glm::rotate(model, toRadians(0), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));


		// View matrix
		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, currentYPos, 0.0f));
		view = glm::rotate(view, toRadians(currentWorldAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, toRadians(180), glm::vec3(0.0f, 1.0f, 0.0f));
		view = camera.calculateViewMatrix() * view;


		// Connect matrices with shaders
		gridShader.setMatrix4Float("model", &model);
		gridShader.setMatrix4Float("projection", &projection);
		gridShader.setMatrix4Float("view", &view);
		meshList[0]->RenderMesh(GL_LINES);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		objectList[0]->SetModelMatrix(model, 0);
		objectList[0]->RenderObject();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.1f, -4.0f));
		model = glm::scale(model, glm::vec3(0.17f, 0.17f, 0.2f));
		objectList[1]->SetModelMatrix(model, 0);
		objectList[1]->RenderObject();

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


void createGrid(int squareCount)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	// Loops through each row, then for each column it creates a point clamped between 0 to 1
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

	// A double loop to connect all the vertices with the appropriate indices.
	// First it top left to top right vertex, then top right with bottom right, then bottom right with bottom left and finally bottom left with top right. 
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

	ComplexObject* letterR = CreateLetterR(modelLocation);
	ComplexObject* letterP = CreateLetterP(modelLocation);
	ComplexObject* number2 = CreateNumber2(modelLocation);
	ComplexObject* number4 = CreateNumber4(modelLocation);
	
	// R translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	letterR->SetModelMatrix(model, modelLocation);
	// P translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
	letterP->SetModelMatrix(model, modelLocation);
	// 2 translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(13.0f, 0.0f, 0.0f));
	number2->SetModelMatrix(model, modelLocation);
	// 4 translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.0f, 0.0f, 0.0f));
	number4->SetModelMatrix(model, modelLocation);

	ComplexObject* razvanNameAndID = new ComplexObject();
	razvanNameAndID->objectList.push_back(letterR);
	razvanNameAndID->objectList.push_back(letterP);
	razvanNameAndID->objectList.push_back(number2);
	razvanNameAndID->objectList.push_back(number4);

	objectList.push_back(razvanNameAndID);
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

ComplexObject* CreateLetterP(GLuint uniformModel)
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

	// LETTER P

	// Creating the base of the letter P
	IndependentMesh* pBase = new IndependentMesh();
	pBase->CreateMesh(vertices, indices, 24, 36);
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 6.0f, 1.0f));

	pBase->SetModelMatrix(model, uniformModel);


	// Creating top part of p
	IndependentMesh* pTop = new IndependentMesh();
	pTop->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);
	
	model = glm::translate(model, glm::vec3(2.0f, 4.5f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
	
	pTop->SetModelMatrix(model, uniformModel);
	
	// Creating right side of top
	IndependentMesh* pTopRight = new IndependentMesh();
	pTopRight->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(3.0f, 3.15f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.7f, 1.0f));

	pTopRight->SetModelMatrix(model, uniformModel);

	// Creating bottom of top
	IndependentMesh* pTopBottom = new IndependentMesh();
	pTopBottom->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(2.0f, 1.8f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));

	pTopBottom->SetModelMatrix(model, uniformModel);


	ComplexObject* pLetter = new ComplexObject();
	pLetter->meshList.push_back(pBase);
	pLetter->meshList.push_back(pTop);
	pLetter->meshList.push_back(pTopRight);
	pLetter->meshList.push_back(pTopBottom);

	return pLetter;
}

ComplexObject* CreateLetterR(GLuint uniformModel)
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

	// LETTER R

	// Creating the base of the letter R
	IndependentMesh* rBase = new IndependentMesh();
	rBase->CreateMesh(vertices, indices, 24, 36);
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 6.0f, 1.0f));

	rBase->SetModelMatrix(model, uniformModel);


	// Creating top part of R
	IndependentMesh* rTop = new IndependentMesh();
	rTop->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(2.0f, 4.5f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));

	rTop->SetModelMatrix(model, uniformModel);

	// Creating right side of top
	IndependentMesh* rTopRight = new IndependentMesh();
	rTopRight->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(3.0f, 3.15f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.7f, 1.0f));

	rTopRight->SetModelMatrix(model, uniformModel);

	// Creating bottom of top
	IndependentMesh* rTopBottom = new IndependentMesh();
	rTopBottom->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(2.0f, 1.8f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));

	rTopBottom->SetModelMatrix(model, uniformModel);


	// Creating bottom 
	IndependentMesh* rBottom = new IndependentMesh();
	rBottom->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(2.0f, 0.5f, 0.0f));
	model = glm::rotate(model, toRadians(45), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, 3.5f, 1.0f));

	rBottom->SetModelMatrix(model, uniformModel);
	
	ComplexObject* rLetter = new ComplexObject();
	rLetter->meshList.push_back(rBase);
	rLetter->meshList.push_back(rTop);
	rLetter->meshList.push_back(rTopRight);
	rLetter->meshList.push_back(rTopBottom);
	rLetter->meshList.push_back(rBottom);


	return rLetter;
}

ComplexObject* CreateNumber2(GLuint uniformModel)
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

	// NUMBER 2

	// Creating the top of the number 2 
	IndependentMesh* twoTop = new IndependentMesh();
	twoTop->CreateMesh(vertices, indices, 24, 36);
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(2.0f, 4.5f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));

	twoTop->SetModelMatrix(model, uniformModel);


	// Creating right side 
	IndependentMesh* twoRightSide = new IndependentMesh();
	twoRightSide->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);


	model = glm::translate(model, glm::vec3(3.0f, 3.15f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.7f, 1.0f));

	twoRightSide->SetModelMatrix(model, uniformModel);

	// Creating middle part
	IndependentMesh* twoMid = new IndependentMesh();
	twoMid->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(2.0f, 1.8f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));

	twoMid->SetModelMatrix(model, uniformModel);

	// Creating left side 
	IndependentMesh* twoLeftSide = new IndependentMesh();
	twoLeftSide->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);


	model = glm::translate(model, glm::vec3(1.0f, 0.5f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.7f, 1.0f));

	twoLeftSide->SetModelMatrix(model, uniformModel);

	// Creating the bottom of the number 2 
	IndependentMesh* twobottom = new IndependentMesh();
	twobottom->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(2.0f, -0.5f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));

	twobottom->SetModelMatrix(model, uniformModel);


	ComplexObject* twoNumber = new ComplexObject();
	twoNumber->meshList.push_back(twoTop);
	twoNumber->meshList.push_back(twoRightSide);
	twoNumber->meshList.push_back(twoMid);
	twoNumber->meshList.push_back(twoLeftSide);
	twoNumber->meshList.push_back(twobottom);


	return twoNumber;
}

ComplexObject* CreateNumber4(GLuint uniformModel)
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

	// NUMBER 4

	// Creating the top of the number 4 
	IndependentMesh* fourTop = new IndependentMesh();
	fourTop->CreateMesh(vertices, indices, 24, 36);
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(1.0f, 3.6f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 2.8f, 1.0f));

	fourTop->SetModelMatrix(model, uniformModel);


	// Creating middle part 
	IndependentMesh* fourMiddle = new IndependentMesh();
	fourMiddle->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);


	model = glm::translate(model, glm::vec3(2.0f, 1.8f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));

	fourMiddle->SetModelMatrix(model, uniformModel);

	// Creating right side
	IndependentMesh* fourRight = new IndependentMesh();
	fourRight->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(3.0f, 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 6.0f, 1.0f));

	fourRight->SetModelMatrix(model, uniformModel);


	ComplexObject* fourNumber = new ComplexObject();
	fourNumber->meshList.push_back(fourTop);
	fourNumber->meshList.push_back(fourMiddle);
	fourNumber->meshList.push_back(fourRight);
	

	return fourNumber;
}

// Draws the characters SA43
void drawSA43(Shader* shaders) {

	ComplexObject* all = new ComplexObject();

	unsigned int indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};


	GLfloat vertices[] = {
		// front
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0
	};

	GLfloat cube_colors[] = {
		// front colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		// back colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0
	};

	GLuint uniformModel = shaders->getLocation("model");

	///////
	// S //
	///////

	ComplexObject* s = new ComplexObject();

	glm::mat4 partModel(1.0f);
	partModel = glm::scale(partModel, glm::vec3(2.75f, 0.5f, 1.0f));
	IndependentMesh* cubeS1 = new IndependentMesh();
	cubeS1->CreateMesh(vertices, indices, 24, 36);
	cubeS1->SetModelMatrix(partModel, uniformModel);
	s->meshList.push_back(cubeS1);

	partModel = glm::translate(partModel, glm::vec3(0.0f, 8.0f, 0.0f));
	IndependentMesh *cubeS2 = new IndependentMesh();
	cubeS2->CreateMesh(vertices, indices, 24, 36);
	cubeS2->SetModelMatrix(partModel, uniformModel);
	s->meshList.push_back(cubeS2);

	partModel = glm::translate(partModel, glm::vec3(0.0f, 7.5f, 0.0f));
	IndependentMesh *cubeS3 = new IndependentMesh();
	cubeS3->CreateMesh(vertices, indices, 24, 36);
	cubeS3->SetModelMatrix(partModel, uniformModel);
	s->meshList.push_back(cubeS3);

	partModel = glm::mat4(1.0f);
	partModel = glm::scale(partModel, glm::vec3(0.5f, 1.8f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(4.5f, 1.2f, -0.1f));
	IndependentMesh *cubeS4 = new IndependentMesh();
	cubeS4->CreateMesh(vertices, indices, 24, 36);
	cubeS4->SetModelMatrix(partModel, uniformModel);
	s->meshList.push_back(cubeS4);

	partModel = glm::translate(partModel, glm::vec3(-9.2f, 1.9f, -0.1f));
	IndependentMesh *cubeS5 = new IndependentMesh();
	cubeS5->CreateMesh(vertices, indices, 24, 36);
	cubeS5->SetModelMatrix(partModel, uniformModel);
	s->meshList.push_back(cubeS5);

	all->objectList.push_back(s);

	glm::mat4 groupModel(1.0f);
	//groupModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	groupModel = glm::rotate(groupModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	groupModel = glm::translate(groupModel, glm::vec3(5.0f, -15.0f, 0.0f));
	s->SetModelMatrix(groupModel, uniformModel);


	///////
	// A //
	///////

	ComplexObject* a = new ComplexObject();

	partModel = glm::mat4(1.0f); // Matrix for each fragment
								 // First fragment
	partModel = glm::scale(partModel, glm::vec3(2.3f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.0f, 15.3f, 0.0f));
	IndependentMesh *cubeA1 = new IndependentMesh();
	cubeA1->CreateMesh(vertices, indices, 24, 36);
	cubeA1->SetModelMatrix(partModel, uniformModel);
	a->meshList.push_back(cubeA1);

	// Third fragment
	partModel = glm::mat4(1.0f);
	partModel = glm::scale(partModel, glm::vec3(2.3f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.0f, 5.3f, 0.0f));
	IndependentMesh *cubeA2 = new IndependentMesh();
	cubeA2->CreateMesh(vertices, indices, 24, 36);
	cubeA2->SetModelMatrix(partModel, uniformModel);
	a->meshList.push_back(cubeA2);

	// Second fragment
	partModel = glm::mat4(1.0f);
	partModel = glm::scale(partModel, glm::vec3(0.5f, 4.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(-5.0f, 0.8f, -0.1f));
	IndependentMesh *cubeA3 = new IndependentMesh();
	cubeA3->CreateMesh(vertices, indices, 24, 36);
	cubeA3->SetModelMatrix(partModel, uniformModel);
	a->meshList.push_back(cubeA3);

	// Fourth fragment
	partModel = glm::mat4(1.0f);
	partModel = glm::scale(partModel, glm::vec3(0.5f, 4.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(5.1f, 0.8f, -0.1f));
	IndependentMesh *cubeA4 = new IndependentMesh();
	cubeA4->CreateMesh(vertices, indices, 24, 36);
	cubeA4->SetModelMatrix(partModel, uniformModel);
	a->meshList.push_back(cubeA4);

	all->objectList.push_back(a);

	groupModel = glm::mat4(1.0f);
	groupModel = glm::translate(groupModel, glm::vec3(-2.8f, 0.2f, 0.0f));

	a->SetModelMatrix(groupModel, uniformModel);

	///////
	// 4 //
	///////

	ComplexObject * four = new ComplexObject();

	// First fragment
	partModel = glm::translate(partModel, glm::vec3(5.3f, 0.0f, 0.0f));
	partModel = glm::scale(partModel, glm::vec3(1.0f, 1.0f, 1.0f));
	IndependentMesh *cube41 = new IndependentMesh();
	cube41->CreateMesh(vertices, indices, 24, 36);
	cube41->SetModelMatrix(partModel, uniformModel);
	four->meshList.push_back(cube41);

	// Third fragment
	partModel = glm::mat4(1.0f);
	partModel = glm::translate(partModel, glm::vec3(2.0f, 5.7f, 0.0f));
	partModel = glm::scale(partModel, glm::vec3(0.5f, 2.3f, 1.0f));
	IndependentMesh *cube42 = new IndependentMesh();
	cube42->CreateMesh(vertices, indices, 24, 36);
	cube42->SetModelMatrix(partModel, uniformModel);
	four->meshList.push_back(cube42);

	// Second fragment
	partModel = glm::mat4(1.0f);
	partModel = glm::translate(partModel, glm::vec3(3.5f, 3.0f, -0.1f));
	partModel = glm::scale(partModel, glm::vec3(2.0f, 0.5f, 1.0f));
	IndependentMesh *cube43 = new IndependentMesh();
	cube43->CreateMesh(vertices, indices, 24, 36);
	cube43->SetModelMatrix(partModel, uniformModel);
	four->meshList.push_back(cube43);

	all->objectList.push_back(four);

	groupModel = glm::mat4(1.0f);
	groupModel = glm::translate(groupModel, glm::vec3(0.0f, 0.2f, 0.0f));

	four->SetModelMatrix(groupModel, uniformModel);

	///////
	// 3 //
	///////

	ComplexObject * three = new ComplexObject();

	// First fragment
	partModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 4.45f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(22.0f, 0.78f, 0.0f));
	IndependentMesh *cube31 = new IndependentMesh();
	cube31->CreateMesh(vertices, indices, 24, 36);
	cube31->SetModelMatrix(partModel, uniformModel);
	three->meshList.push_back(cube31);

	// Third fragment
	partModel = glm::mat4(1.0f);
	partModel = glm::scale(partModel, glm::vec3(2.0f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(4.5f, 14.9f, -0.1f));
	IndependentMesh *cube32 = new IndependentMesh();
	cube32->CreateMesh(vertices, indices, 24, 36);
	cube32->SetModelMatrix(partModel, uniformModel);
	three->meshList.push_back(cube32);

	// Second fragment
	partModel = glm::translate(partModel, glm::vec3(0.0f, -7.9f, -0.1f));
	IndependentMesh *cube33 = new IndependentMesh();
	cube33->CreateMesh(vertices, indices, 24, 36);
	cube33->SetModelMatrix(partModel, uniformModel);
	three->meshList.push_back(cube33);

	partModel = glm::translate(partModel, glm::vec3(0.0f, -7.9f, -0.1f));
	IndependentMesh *cube34 = new IndependentMesh();
	cube34->CreateMesh(vertices, indices, 24, 36);
	cube34->SetModelMatrix(partModel, uniformModel);
	three->meshList.push_back(cube34);

	all->objectList.push_back(three);

	groupModel = glm::mat4(1.0f);
	groupModel = glm::translate(groupModel, glm::vec3(0.0f, 0.2f, 0.0f));

	three->SetModelMatrix(groupModel, uniformModel);

	groupModel = glm::mat4(1.0f);
	groupModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	groupModel = glm::rotate(groupModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	all->SetModelMatrix(groupModel, uniformModel);

	s->RenderObject();

	objectList.push_back(all);

}