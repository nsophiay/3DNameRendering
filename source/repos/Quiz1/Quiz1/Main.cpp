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
// - Sphere http://www.songho.ca/opengl/gl_sphere.html

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
/// <returns>A pointer to the complex object representing the letter I</returns>
ComplexObject* CreateLetterI(GLuint uniformModel);

/// <summary>
/// Creates the letter N using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the letter N</returns>
ComplexObject* CreateLetterN(GLuint uniformModel);

/// <summary>
/// Creates the letter R using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the letter R</returns>
ComplexObject* CreateLetterR(GLuint uniformModel);

/// <summary>
/// Creates the letter O using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the letter O</returns>
ComplexObject* CreateLetterO(GLuint uniformModel);

ComplexObject* CreateCylinder(int sectorCount, float height, float radius);
std::vector<float> getUnitCircleVertices(int sectorCount);

void CreateAxes();

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

	// Create the axes
	CreateAxes();

	glm::mat4 model;

    // Setup default matrix - Saffia
    model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.1f, -10.0f));
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
        if(selectedModel == 0){
            objectList[0]->objectList[0]->Transform(window.getKeys());
        }
		if (selectedModel == 1) {
			objectList[0]->objectList[1]->Transform(window.getKeys());
		}
		if (selectedModel == 2) {
			objectList[0]->objectList[2]->Transform(window.getKeys());
		}
		if (selectedModel == 3) {
			objectList[0]->objectList[3]->Transform(window.getKeys());
		}
		if (selectedModel == 4) {
			objectList[0]->objectList[4]->Transform(window.getKeys());
		}
		if (selectedModel == 5) {
			objectList[0]->objectList[5]->Transform(window.getKeys());
		}
		objectList[0]->RenderObject(gridShader);


		// Resetting the matrix
		model = glm::mat4(1.0f);
		gridShader.setMatrix4Float("model", &model);

		// Render the set of axis

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.25f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		objectList[1]->objectList[0]->SetColour(1.0f, 0.0f, 0.0f); // Red
		objectList[1]->objectList[0]->SetModelMatrix(model, 0);
		objectList[1]->objectList[0]->RenderObject(gridShader);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 1.25f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		objectList[1]->objectList[1]->SetColour(0.0f, 1.0f, 0.0f); // Green
		objectList[1]->objectList[1]->SetModelMatrix(model, 0);
		objectList[1]->objectList[1]->RenderObject(gridShader);

		model = glm::mat4(1.0f);
		objectList[1]->objectList[2]->SetColour(0.0f, 0.0f, 1.0f); // Blue
		objectList[1]->objectList[2]->SetModelMatrix(model, 0);
		objectList[1]->objectList[2]->RenderObject(gridShader);

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

///////////////////////////////////////////////////////////
// Source: http://www.songho.ca/opengl/gl_cylinder.html. //
// generate a unit circle on XY-plane
std::vector<float> getUnitCircleVertices(int sectorCount)
{
	const float PI = 3.1415926f;
	float sectorStep = 2 * glm::pi<float>() / sectorCount;
	float sectorAngle;  // radian

	std::vector<float> unitCircleVertices;
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		unitCircleVertices.push_back(cos(sectorAngle)); // x
		unitCircleVertices.push_back(sin(sectorAngle)); // y
		unitCircleVertices.push_back(0);                // z
	}
	return unitCircleVertices;
}
///////////////////////////////////////////////////////////

ComplexObject* CreateCylinder(int sectorCount, float height, float radius) {

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	///////////////////////////////////////////////////////////
	// Source: http://www.songho.ca/opengl/gl_cylinder.html. //


	// get unit circle vectors on XY-plane
	std::vector<float> unitVertices = getUnitCircleVertices(sectorCount);

	// put side vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float t = 1.0f - i;                              // vertical tex coord; 1 to 0

		for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			float uz = unitVertices[k + 2];
			// position vector
			vertices.push_back(ux * radius);             // vx
			vertices.push_back(uy * radius);             // vy
			vertices.push_back(h);                       // vz
		}
	}

	// the starting index for the base/top surface
	//NOTE: it is used for generating indices later
	int baseCenterIndex = (int)vertices.size() / 3;
	int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

															// put base and top vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float nz = -1 + i * 2;                           // z value of normal; -1 to 1

														 // center point
		vertices.push_back(0);     vertices.push_back(0);     vertices.push_back(h);

		for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			// position vector
			vertices.push_back(ux * radius);             // vx
			vertices.push_back(uy * radius);             // vy
			vertices.push_back(h);                       // vz
														 // normal vector
		}
	}


	// generate CCW index list of cylinder triangles
	int k1 = 0;                         // 1st vertex index at base
	int k2 = sectorCount + 1;           // 1st vertex index at top

										// indices for the side surface
	for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
	{
		// 2 triangles per sector
		// k1 => k1+1 => k2
		indices.push_back(k1);
		indices.push_back(k1 + 1);
		indices.push_back(k2);

		// k2 => k1+1 => k2+1
		indices.push_back(k2);
		indices.push_back(k1 + 1);
		indices.push_back(k2 + 1);
	}

	// indices for the base surface
	for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(k + 1);
			indices.push_back(k);
		}
		else // last triangle
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(baseCenterIndex + 1);
			indices.push_back(k);
		}
	}

	// indices for the top surface
	for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(k + 1);
		}
		else // last triangle
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(topCenterIndex + 1);
		}
	}
	
	///////////////////////////////////////////////////////////

	IndependentMesh* m = new IndependentMesh();
	m->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	ComplexObject* cylinder = new ComplexObject();
	cylinder->meshList.push_back(m);

	return cylinder;

}



// Create sphere
IndependentMesh* CreateSphere(float radius, int longitudeCount, int latitudeCount, GLuint modelLocation) {

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
	ComplexObject* letterN = CreateLetterN(modelLocation);
	ComplexObject* letterI = CreateLetterI(modelLocation);
	ComplexObject* letterR = CreateLetterR(modelLocation);
	ComplexObject* letterO = CreateLetterO(modelLocation);
	 
	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(0.5f, 30.0f, 0.0f));
	letterS->SetModelMatrix(model, modelLocation);
	letterS->SetColour(0.96f, 1.0f, 0.0f); // Yellow
	
   	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.5f, 24.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
	letterA->SetModelMatrix(model, modelLocation);
	letterA->SetColour(1.0f, 0.8f, 0.0f); // Gold
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 18.0f, 0.0f));
	letterN->SetModelMatrix(model, modelLocation);
	letterN->SetColour(1.0f, 0.6f, 0.0f); // Orange
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.5f, 12.0f, 0.0f));
	letterI->SetModelMatrix(model, modelLocation);
	letterI->SetColour(0.98f, 0.39f, 0.0f); // Dark Orange
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 6.5f, 0.0f));
	letterR->SetModelMatrix(model, modelLocation);
	letterR->SetColour(0.92f, 0.07f, 0.07f); // Red

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	letterO->SetModelMatrix(model, modelLocation);
	letterO->SetColour(0.71f, 0.0f, 0.0f); // Dark red
	
	ComplexObject* SaffiaNameAndID = new ComplexObject();

	SaffiaNameAndID->objectList.push_back(letterS);
	SaffiaNameAndID->objectList.push_back(letterA);
	SaffiaNameAndID->objectList.push_back(letterN);
	SaffiaNameAndID->objectList.push_back(letterI);
	SaffiaNameAndID->objectList.push_back(letterR);
	SaffiaNameAndID->objectList.push_back(letterO);
	
	objectList.push_back(SaffiaNameAndID);

}


ComplexObject* CreateLetterR(GLuint uniformModel)
{
	
	ComplexObject *r = new ComplexObject();

	// Use spheres for the vertical portions

	glm::mat4 partModel(1.0f);
	IndependentMesh *sphereR1 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::scale(sphereR1->GetModelMatrix(), glm::vec3(1.0f, 1.2f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.5f, 0.9f, 0.0f));
	sphereR1->SetModelMatrix(partModel, uniformModel);
	r->meshList.push_back(sphereR1);

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereR2 = CreateSphere(1.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereR2->GetModelMatrix(), glm::vec3(5.4f, 2.75f, 0.0f));
	sphereR2->SetModelMatrix(partModel, uniformModel);
	r->meshList.push_back(sphereR2);


	// Use cubes for the horizontal portion

	IndependentMesh *cubeR1 = CreateCube(uniformModel);
	partModel = glm::scale(cubeR1->GetModelMatrix(), glm::vec3(3.5f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.75f, 6.5f, 0.0f));
	cubeR1->SetModelMatrix(partModel, uniformModel);
	r->meshList.push_back(cubeR1);

	IndependentMesh *cubeR2 = CreateCube(uniformModel);
	partModel = glm::scale(cubeR2->GetModelMatrix(), glm::vec3(3.5f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.75f, 3.0f, 0.0f));
	cubeR2->SetModelMatrix(partModel, uniformModel);
	r->meshList.push_back(cubeR2);

	partModel = glm::mat4(1.0f);
	IndependentMesh* cubeR3 = CreateCube(uniformModel);
	partModel = glm::translate(cubeR3->GetModelMatrix(), glm::vec3(3.8f, 0.0f, 0.0f));
	partModel = glm::rotate(partModel, toRadians(65), glm::vec3(0.0f, 0.0f, 1.0f));
	partModel = glm::scale(partModel, glm::vec3(0.5f, 2.0f, 1.0f));
	cubeR3->SetModelMatrix(partModel, uniformModel);
	r->meshList.push_back(cubeR3);

	return r;
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

	// LETTER A
	ComplexObject* a = new ComplexObject();

	glm::mat4 partModel(1.0f); 
	
	// Use cubes for horizontal portions

	IndependentMesh *cubeA1 = CreateCube(uniformModel);
	partModel = glm::scale(cubeA1->GetModelMatrix(), glm::vec3(4.0f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.0f, 5.0f, 0.0f));
	cubeA1->SetModelMatrix(partModel, uniformModel);
	a->meshList.push_back(cubeA1);

	partModel = glm::mat4(1.0f);
	IndependentMesh *cubeA2 = CreateCube(uniformModel);
	partModel = glm::scale(cubeA2->GetModelMatrix(), glm::vec3(4.0f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.0f, 10.5f, 0.0f));
	cubeA2->SetModelMatrix(partModel, uniformModel);
	a->meshList.push_back(cubeA2);

	// Use spheres for vertical portions
	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereA3 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereA3->GetModelMatrix(), glm::vec3(-5.0f, 0.8f, -0.1f));
	sphereA3->SetModelMatrix(partModel, uniformModel);
	a->meshList.push_back(sphereA3);

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereA4 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereA4->GetModelMatrix(), glm::vec3(5.1f, 0.8f, -0.1f));
	sphereA4->SetModelMatrix(partModel, uniformModel);
	a->meshList.push_back(sphereA4);

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereA5 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereA5->GetModelMatrix(), glm::vec3(-5.0f, 4.0f, -0.1f));
	sphereA5->SetModelMatrix(partModel, uniformModel);
	a->meshList.push_back(sphereA5);

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereA6 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereA6->GetModelMatrix(), glm::vec3(5.1f, 4.0f, -0.1f));
	sphereA6->SetModelMatrix(partModel, uniformModel);
	a->meshList.push_back(sphereA6);

	return a;
}

ComplexObject* CreateLetterI(GLuint uniformModel)
{

    ComplexObject *i = new ComplexObject();

	// Use cubes for the horizontal portion

    glm::mat4 partModel(1.0f);
	IndependentMesh *cubeI1 = CreateCube(uniformModel);
	partModel = glm::scale(cubeI1->GetModelMatrix(), glm::vec3(3.5f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.0f, 8.25f, 0.0f));
	cubeI1->SetModelMatrix(partModel, uniformModel);
	i->meshList.push_back(cubeI1);

	IndependentMesh *cubeI3 = CreateCube(uniformModel);
	partModel = glm::scale(cubeI3->GetModelMatrix(), glm::vec3(3.5f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(0.0f, -1.0f, 0.0f));
	cubeI3->SetModelMatrix(partModel, uniformModel);
	i->meshList.push_back(cubeI3);

	// Use spheres for the vertical portion

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereI2 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereI2->GetModelMatrix(), glm::vec3(0.0f, 1.75f, 1.0f));
	sphereI2->SetModelMatrix(partModel, uniformModel);
	i->meshList.push_back(sphereI2);

    return i;
}

ComplexObject* CreateLetterN(GLuint uniformModel)
{

    ComplexObject *n = new ComplexObject();

	// Use spheres for the vertical portions

    glm::mat4 partModel(1.0f);
    IndependentMesh *sphereN1 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereN1->GetModelMatrix(), glm::vec3(0.0f, 0.9f, 0.0f));
	sphereN1->SetModelMatrix(partModel, uniformModel);
    n->meshList.push_back(sphereN1);

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereN3 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereN3->GetModelMatrix(), glm::vec3(6.0f, 0.9f, 0.0f));
	sphereN3->SetModelMatrix(partModel, uniformModel);
	n->meshList.push_back(sphereN3);

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereN4 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereN4->GetModelMatrix(), glm::vec3(0.0f, 3.0f, 0.0f));
	sphereN4->SetModelMatrix(partModel, uniformModel);
	n->meshList.push_back(sphereN4);

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereN5 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereN5->GetModelMatrix(), glm::vec3(6.0f, 3.0f, 0.0f));
	sphereN5->SetModelMatrix(partModel, uniformModel);
	n->meshList.push_back(sphereN5);


	// Use cubes for the horizontal portion

	IndependentMesh *cubeN2 = CreateCube(uniformModel);
	partModel = glm::scale(cubeN2->GetModelMatrix(), glm::vec3(3.5f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(1.0f, 8.75f, 0.0f));
	cubeN2->SetModelMatrix(partModel, uniformModel);
	n->meshList.push_back(cubeN2);

    return n;
}

ComplexObject* CreateLetterO(GLuint uniformModel)
{

	ComplexObject *o = new ComplexObject();

	// Use spheres for the vertical portions

	glm::mat4 partModel(1.0f);
	IndependentMesh *sphereN1 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereN1->GetModelMatrix(), glm::vec3(0.0f, 0.9f, 0.0f));
	sphereN1->SetModelMatrix(partModel, uniformModel);
	o->meshList.push_back(sphereN1);

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereN3 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereN3->GetModelMatrix(), glm::vec3(6.0f, 0.9f, 0.0f));
	sphereN3->SetModelMatrix(partModel, uniformModel);
	o->meshList.push_back(sphereN3);

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereN4 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereN4->GetModelMatrix(), glm::vec3(0.0f, 3.0f, 0.0f));
	sphereN4->SetModelMatrix(partModel, uniformModel);
	o->meshList.push_back(sphereN4);

	partModel = glm::mat4(1.0f);
	IndependentMesh *sphereN5 = CreateSphere(2.0, 40, 40, uniformModel);
	partModel = glm::translate(sphereN5->GetModelMatrix(), glm::vec3(6.0f, 3.0f, 0.0f));
	sphereN5->SetModelMatrix(partModel, uniformModel);
	o->meshList.push_back(sphereN5);


	// Use cubes for the horizontal portion

	IndependentMesh *cubeN2 = CreateCube(uniformModel);
	partModel = glm::scale(cubeN2->GetModelMatrix(), glm::vec3(3.5f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(1.0f, 8.75f, 0.0f));
	cubeN2->SetModelMatrix(partModel, uniformModel);
	o->meshList.push_back(cubeN2);

	IndependentMesh *cubeN6 = CreateCube(uniformModel);
	partModel = glm::scale(cubeN6->GetModelMatrix(), glm::vec3(3.5f, 0.5f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(1.0f, -1.0f, 0.0f));
	cubeN6->SetModelMatrix(partModel, uniformModel);
	o->meshList.push_back(cubeN6);

	return o;
}


void CreateAxes()
{

	ComplexObject *axes = new ComplexObject();

	// Create 3 cylinders, one for each axis
	ComplexObject *x = CreateCylinder(5, 2.5f, 0.125f);
	ComplexObject *y = CreateCylinder(5, 2.5f, 0.125f);
	ComplexObject *z = CreateCylinder(5, 2.5f, 0.125f);

	// Add them to the complex object of the entire axis
	axes->objectList.push_back(x);
	axes->objectList.push_back(y);
	axes->objectList.push_back(z);
	
	// Add to total object list
	objectList.push_back(axes);

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
	if(keys[GLFW_KEY_6]) selectedModel = 5;

}