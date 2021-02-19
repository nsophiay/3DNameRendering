#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Window.h"
#include "IndependentMesh.h"
#include "ComplexObject.h"
#include "Cylinder.h"

// References used:
// - Cylinder http://www.songho.ca/opengl/gl_cylinder.html#example_cylinder
// - Sphere https://gist.github.com/zwzmzd/0195733fa1210346b00d

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
/// Creates the letter S using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the letter S</returns>
ComplexObject* CreateLetterS(GLuint uniformModel);
/// <summary>
/// Creates the letter A using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the letter A</returns>
ComplexObject* CreateLetterA(GLuint uniformModel);

/// <summary>
/// Creates the number I using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the number 3</returns>
ComplexObject* CreateLetterI(GLuint uniformModel);

/// <summary>
/// Creates the letter N using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the number 3</returns>
ComplexObject* CreateLetterN(GLuint uniformModel);

void CreateCylinder();

void CreateAxises(Shader* shader);

/// <summary>
// Reads keyboard input and sets selectedModel to desired input.
// </summary>
void SelectModel();

// Global Variables

const int WIDTH = 1024, HEIGHT = 768;
std::vector<Mesh*> meshList;
std::vector<ComplexObject*> objectList;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 0.05f, 0.5f); // Initialize camera
Window window;

const float BASE_WORLD_XANGLE = -5.0f;
const float BASE_WORLD_YANGLE = 0.0f;
const float BASE_WORLD_Y_POS = -0.5f;
float currentWorldXAngle = BASE_WORLD_XANGLE;
float currentWorldYAngle = BASE_WORLD_YANGLE;
float currentYPos = BASE_WORLD_Y_POS;
float worldRotationIncrement = 0.5f;
float worldPosIncrement = 0.01f;

unsigned int selectedModel = 0; // Selected model to transform using keyboard

int main(int argc, char* argv[])
{
	// Initializing Global Variables
	meshList = std::vector<Mesh*>();
	objectList = std::vector<ComplexObject*>();

	window = Window(WIDTH, HEIGHT);
	window.initialise();

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.0f, 0.52f, 0.52f, 1.0f);

	// Creating grid
	createGrid(128);
	Shader gridShader = Shader("shader.vs", "shader.fs");

	glm::mat4 projection(1.0f);
	projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	// Creating the letters
	CreateLetters(&gridShader);

	// Create the axises
	CreateAxises(&gridShader);

	glm::mat4 model;

    // Setup default matrix - Saffia
    model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 0.58f, -1.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    objectList[0]->SetModelMatrix(model, 0);


	while (!window.getShouldClose())
	{

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Camera movement
		camera.pan(window.getKeys(), window.getDeltaX());
		camera.tilt(window.getKeys(), window.getDeltaY());
		camera.magnify(window.getKeys(), window.getDeltaY());
		camera.movementFromKeyboard(window.getKeys());

		gridShader.use();

		// Handling rotations
		// Rotating the entire world dependent on key presses.
			// We rotate around the X-Axis
		if (window.getKeys()[GLFW_KEY_LEFT])
		{
			// Anticlockwise rotation
			currentWorldXAngle += worldRotationIncrement;
		}
		if (window.getKeys()[GLFW_KEY_RIGHT])
		{
			// Clockwise rotation
			currentWorldXAngle -= worldRotationIncrement;
		}
		if (window.getKeys()[GLFW_KEY_UP])
		{
			// Anticlockwise rotation
			currentWorldYAngle += worldRotationIncrement;
		}
		if (window.getKeys()[GLFW_KEY_DOWN])
		{
			// Clockwise rotation
			currentWorldYAngle -= worldRotationIncrement;
		}
		if (window.getKeys()[GLFW_KEY_HOME])
		{
			// Reset to default rotation.
			currentWorldXAngle = BASE_WORLD_XANGLE;
			currentWorldYAngle = BASE_WORLD_YANGLE;
		}

		// Handling vertical camera movement
		if (window.getKeys()[GLFW_KEY_EQUAL])
		{
			currentYPos -= worldPosIncrement;
		}
		if (window.getKeys()[GLFW_KEY_MINUS])
		{
			currentYPos += worldPosIncrement;
		}
		
		if (window.getKeys()[GLFW_KEY_T])
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (window.getKeys()[GLFW_KEY_L] && !window.getKeys()[GLFW_KEY_LEFT_SHIFT])
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (window.getKeys()[GLFW_KEY_P])
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}

        SelectModel();

		// Model matrix for the world grid
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -10.0f));
		model = glm::rotate(model, toRadians(0), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));


		// View matrix
		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, currentYPos, 2.3f));
		view = glm::rotate(view, toRadians(currentWorldXAngle), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotating around X Axis
		view = glm::rotate(view, toRadians(currentWorldYAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotating around Y Axis
		view = glm::rotate(view, toRadians(180), glm::vec3(0.0f, 1.0f, 0.0f));
		view = camera.calculateViewMatrix() * view;


		// Connect matrices with shaders
		gridShader.setMatrix4Float("model", &model);
		gridShader.setMatrix4Float("projection", &projection);
		gridShader.setMatrix4Float("view", &view);

		// Drawing the grid
		// Setting the color (yellow)
		gridShader.setFloat("r", 0.8f);
		gridShader.setFloat("rg", 0.85f);
		gridShader.setFloat("rgb", 0.0f);
		meshList[0]->RenderMesh(GL_LINES);

		// Drawing the letters
		// Setting color to gray
		gridShader.setFloat("r", 0.55f);
		gridShader.setFloat("rg", 0.55f);
		gridShader.setFloat("rgb", 0.55f);

	

		// Saffia
        // If 3 is pressed, transform model with keyboard
        if(selectedModel == 2){
            objectList[0]->Transform(window.getKeys());
        }
		objectList[0]->RenderObject();


		// Resetting the matrix
		model = glm::mat4(1.0f);
		gridShader.setMatrix4Float("model", &model);

		CreateCylinder();

		// Render the set of axis
		// Setting the colors
		gridShader.setFloat("r", 1.0); // Red
		gridShader.setFloat("rg", 0.0f); // Green
		gridShader.setFloat("rgb", 0.0f); // Blue
		objectList[2]->RenderObject();

		gridShader.setFloat("r", 0.0f);
		gridShader.setFloat("rg", 1.0f);
		gridShader.setFloat("rgb", 0.0f);
		objectList[2]->RenderObject();

		gridShader.setFloat("r", 0.0f);
		gridShader.setFloat("rg", 0.0f);
		gridShader.setFloat("rgb", 1.0f);
		objectList[2]->RenderObject();

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

void CreateCylinder() {

	ComplexObject* cylinder = new ComplexObject();

	// Cylinder with diameter 0.25 and length 2.5
	//Cylinder cr(0.125f, 0.125f, 2.5f, 36, 8, false); // baseRadius, topRadius, height, sectors, stacks, flat shading
	Cylinder cr(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, false);

	IndependentMesh *c = new IndependentMesh();
	c->CreateCylinder(cr);

	objectList.push_back(cylinder);

}

// Create sphere
IndependentMesh* CreateSphere(int radius, int longitudeCount, int latitudeCount, GLuint modelLocation) {

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	//////////////////////////////////////////////////////////
	// Source: http://www.songho.ca/opengl/gl_sphere.html. //

	// Generate vertices
	float x, y, z, xy;                              // vertex position

	float sectorStep = 2 * glm::pi<float>() / longitudeCount;
	float stackStep = glm::pi<float>() / latitudeCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= latitudeCount; ++i)
	{
		stackAngle = glm::pi<float>() / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

													// add (sectorCount+1) vertices per stack
													// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= longitudeCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

													// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	// Generate indices
	std::vector<int> lineIndices;
	int k1, k2;
	for (int i = 0; i < latitudeCount; ++i)
	{
		k1 = i * (longitudeCount + 1);     // beginning of current stack
		k2 = k1 + longitudeCount + 1;      // beginning of next stack

		for (int j = 0; j < longitudeCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (latitudeCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

			// store indices for lines
			// vertical lines for all stacks, k1 => k2
			lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
		}
	}
	//////////////////////////////////////////////////////////

	// Half-unit tall, 1 unit wide, 0.25 units deep
	glm::mat4 sizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.0f, 0.25f));

	IndependentMesh* sphere = new IndependentMesh();
	sphere->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	sphere->SetModelMatrix(sizeMatrix, modelLocation);
	return sphere;
	
}

// Create cube
IndependentMesh* CreateCube(GLuint modelLocation) {


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
	

	// Half-unit tall, 1 unit wide, 0.25 units deep
	glm::mat4 sizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.0f, 0.25f));

	IndependentMesh* cube = new IndependentMesh();
	cube->CreateMesh(vertices, indices, 24, 36);
	cube->SetModelMatrix(sizeMatrix, modelLocation);
	return cube;

}


void CreateLetters(Shader* shader) {
	GLuint modelLocation = shader->getLocation("model");


	//////////////////////////////////////////
	// Creating Saffia's name and ID object //
	//////////////////////////////////////////

	ComplexObject* letterS = CreateLetterS(modelLocation);
	ComplexObject* letterA = CreateLetterA(modelLocation);
	 
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f));
	letterS->SetModelMatrix(model, modelLocation);

   	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(6.5f, 0.0f, -2.5f));
	model = glm::scale(model, glm::vec3(1.0f, 0.98f, 1.0f));
	letterA->SetModelMatrix(model, modelLocation);


	ComplexObject* SaffiaNameAndID = new ComplexObject();
	SaffiaNameAndID->objectList.push_back(letterS);
	SaffiaNameAndID->objectList.push_back(letterA);

	objectList.push_back(SaffiaNameAndID);


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



ComplexObject* CreateLetterS(GLuint uniformModel)
{
	// LETTER S
	ComplexObject* s = new ComplexObject();

	// Use cubes for the horizontal portions

	glm::mat4 partModel(1.0f);
	IndependentMesh* cubeS1 = CreateCube(uniformModel);
	partModel = glm::scale(cubeS1->GetModelMatrix(), glm::vec3(2.75f, 0.5f, 1.0f));
	cubeS1->SetModelMatrix(partModel, uniformModel);
	s->meshList.push_back(cubeS1);

	partModel = glm::mat4(1.0f);
	IndependentMesh *cubeS2 = CreateCube(uniformModel);
	partModel = glm::scale(cubeS2->GetModelMatrix(), glm::vec3(2.75f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.0f, 4.5f, 0.0f));
	cubeS2->SetModelMatrix(partModel, uniformModel);
	s->meshList.push_back(cubeS2);

	partModel = glm::mat4(1.0f);
	IndependentMesh *cubeS3 = CreateCube(uniformModel);
	partModel = glm::scale(cubeS3->GetModelMatrix(), glm::vec3(2.75f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.0f, 9.25f, 0.0f));
	cubeS3->SetModelMatrix(partModel, uniformModel);
	s->meshList.push_back(cubeS3);

	// Use spheres for the vertical portions

	partModel = glm::mat4(1.0f);
	IndependentMesh* sphereS1 = CreateSphere(1.75, 40, 40, uniformModel);
	partModel = glm::translate(sphereS1->GetModelMatrix(), glm::vec3(2.25f, 1.0f, -0.1f));
	sphereS1->SetModelMatrix(partModel, uniformModel);
	s->meshList.push_back(sphereS1);

	partModel = glm::mat4(1.0f);
	IndependentMesh* sphereS2 = CreateSphere(1.75, 40, 40, uniformModel);
	partModel = glm::translate(sphereS2->GetModelMatrix(), glm::vec3(-2.0f, 3.5f, -0.1f));
	sphereS2->SetModelMatrix(partModel, uniformModel);
	s->meshList.push_back(sphereS2);

	return s;
}
ComplexObject* CreateLetterA(GLuint uniformModel)
{
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

	// LETTER A
	ComplexObject* a = new ComplexObject();

	glm::mat4 partModel(1.0f); 
	
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

	return a;
}

ComplexObject* CreateLetterI(GLuint uniformModel)
{
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
    ///////
    // I //
    ///////

    ComplexObject *i = new ComplexObject();
    glm::mat4 partModel(1.0f);
    partModel = glm::scale(partModel, glm::vec3(2.75f, 0.5f, 1.0f));
    IndependentMesh *cubeI1 = new IndependentMesh();
    cubeI1->CreateMesh(vertices, indices, 24, 36);
    cubeI1->SetModelMatrix(partModel, uniformModel);
    i->meshList.push_back(cubeI1);

    partModel = glm::translate(partModel, glm::vec3(0.0f, 14.0f, 1.0f));
    IndependentMesh *cubeI2 = new IndependentMesh();
    cubeI2->CreateMesh(vertices, indices, 24, 36);
    cubeI2->SetModelMatrix(partModel, uniformModel);
    i->meshList.push_back(cubeI2);

    partModel = glm::mat4(1.0f);
    partModel = glm::scale(partModel, glm::vec3(0.5f, 3.5f, 1.0f));
    partModel = glm::translate(partModel, glm::vec3(0.0f, 1.0f, 0.0f));
    IndependentMesh *cubeI3 = new IndependentMesh();
    cubeI3->CreateMesh(vertices, indices, 24, 36);
    cubeI3->SetModelMatrix(partModel, uniformModel);
    i->meshList.push_back(cubeI3);

    return i;
}

ComplexObject* CreateLetterN(GLuint uniformModel)
{
    ///////
    // N //
    ///////
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
    ComplexObject *n = new ComplexObject();
    glm::mat4 partModel(1.0f);
    partModel = glm::scale(partModel, glm::vec3(0.5f, 3.9f, 1.0f));
    partModel = glm::translate(partModel, glm::vec3(0.0f, 0.9f, 0.0f));
    IndependentMesh *cubeN1 = new IndependentMesh();
    cubeN1->CreateMesh(vertices, indices, 24, 36);
    cubeN1->SetModelMatrix(partModel, uniformModel);
    n->meshList.push_back(cubeN1);

    partModel = glm::translate(partModel, glm::vec3(9.45f, 0.0f, 0.0f));
    IndependentMesh *cubeN2 = new IndependentMesh();
    cubeN2->CreateMesh(vertices, indices, 24, 36);
    cubeN2->SetModelMatrix(partModel, uniformModel);
    n->meshList.push_back(cubeN2);

    partModel = glm::mat4(1.0f);
    partModel = glm::scale(partModel, glm::vec3(2.75f, 0.5f, 1.0f));
    partModel = glm::translate(partModel, glm::vec3(0.9f, 14.0f, 0.0f));
    IndependentMesh *cubeN4 = new IndependentMesh();
    cubeN4->CreateMesh(vertices, indices, 24, 36);
    cubeN4->SetModelMatrix(partModel, uniformModel);
    n->meshList.push_back(cubeN4);

    return n;
}


void CreateAxises(Shader* shader)
{
	GLuint modelLocation = shader->getLocation("model");

	// 4 points for our axises
	GLfloat vertices[] = {
		0.0, 0.0, 0.0,
		7.0, 0.0, 0.0,
		0.0, 7.0, 0.0,
		0.0, 0.0, 7.0
	};

	// We draw 3 lines.
	unsigned int indicesX[] = {
		0, 1
	};

	unsigned int indicesY[] = {
		0, 2
	};

	unsigned int indicesZ[] = {
		0, 3
	};

	ComplexObject* axises = new ComplexObject();

	// Moving the set of axis
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.005f, 0.0f));
	model = glm::rotate(model, glm::radians(-0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.156f, 0.156f, 0.156f));

	IndependentMesh* objX = new IndependentMesh();
	objX->CreateMesh(vertices, indicesX, 12, 2);
	objX->SetModelMatrix(model, modelLocation);
	axises->meshList.push_back(objX);

	IndependentMesh* objY = new IndependentMesh();
	objY->CreateMesh(vertices, indicesY, 12, 2);
	objY->SetModelMatrix(model, modelLocation);
	axises->meshList.push_back(objY);

	IndependentMesh* objZ = new IndependentMesh();
	objZ->CreateMesh(vertices, indicesZ, 12, 2);
	objZ->SetModelMatrix(model, modelLocation);
	axises->meshList.push_back(objZ);

	objectList.push_back(axises);
}

void SelectModel()
{
    bool *keys = window.getKeys();
    // Return index of model in ComplexObject vector
    if(keys[GLFW_KEY_1]) selectedModel = 0;
    if(keys[GLFW_KEY_2]) selectedModel = 1;
    if(keys[GLFW_KEY_3]) selectedModel = 2;
    if(keys[GLFW_KEY_4]) selectedModel = 3;
    if(keys[GLFW_KEY_5]) selectedModel = 4;

}
