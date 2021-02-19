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
/// Creates the number 4 using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the number 4</returns>
ComplexObject* CreateNumber4alt(GLuint uniformModel);
/// <summary>
/// Creates the number 3 using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the number 3</returns>
ComplexObject* CreateNumber3(GLuint uniformModel);

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

/// <summary>
/// Creates the number 6 using meshes and complex objects.
/// </summary>
/// <param name="uniformModel">The location of the Model Matrix on the GPU</param>
/// <returns>A pointer to the complex object representing the number 3</returns>
ComplexObject* CreateNumber6(GLuint uniformModel);

ComplexObject* CreateNumber4Michael(GLuint uniformModel);
ComplexObject* CreateNumber7(GLuint uniformModel);
ComplexObject* CreateLetterM(GLuint uniformModel);

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

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Creating grid
	createGrid(128);
	Shader gridShader = Shader("shader.vs", "shader.fs");

	glm::mat4 projection(1.0f);
	projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	// Creating the letters
	CreateLetters(&gridShader);

	// Create the axises
	CreateAxises(&gridShader);

	// Setup default matrix - Joel
	glm::mat4 model;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(7.8f, 0.0f, -8.3f));
    model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    objectList[0]->SetModelMatrix(model, 0);

    // Setup default matrix - Razvan
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-7.2f, 0.1f, 9.0f));
    model = glm::scale(model, glm::vec3(0.17f, 0.17f, 0.2f));
    model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    objectList[1]->SetModelMatrix(model, 0);

    // Setup default matrix - Saffia
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(8.5f, 0.1f, 7.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    objectList[2]->SetModelMatrix(model, 0);

    // Setup default matrix - Ian
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-8.5f, 0.1f, -7.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    objectList[3]->SetModelMatrix(model, 0);

	// Setup default matrix - Michael
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 0.58f, -1.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	objectList[4]->SetModelMatrix(model, 0);


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

		// Joel
        // If 1 is pressed, transform model with keyboard
        if(selectedModel == 0){
            objectList[0]->Transform(window.getKeys());
        }
		objectList[0]->RenderObject();

		// Razvan
        // If 2 is pressed, transform model with keyboard
        if(selectedModel == 1){
            objectList[1]->Transform(window.getKeys());
        }
		objectList[1]->RenderObject();

		// Saffia
        // If 3 is pressed, transform model with keyboard
        if(selectedModel == 2){
            objectList[2]->Transform(window.getKeys());
        }
		objectList[2]->RenderObject();

        // Ian
        // If 4 is pressed, transform model with keyboard
        if(selectedModel == 3){
            objectList[3]->Transform(window.getKeys());
        }
        objectList[3]->RenderObject();

		// Michael
		// If 5 is pressed, transform model with keyboard
		if (selectedModel == 4) {
			objectList[4]->Transform(window.getKeys());
		}
		objectList[4]->RenderObject();

		// Resetting the matrix
		model = glm::mat4(1.0f);
		gridShader.setMatrix4Float("model", &model);

		// Render the set of axis
		// Setting the colors
		gridShader.setFloat("r", 1.0);
		gridShader.setFloat("rg", 0.0f);
		gridShader.setFloat("rgb", 0.0f);
		objectList[5]->meshList[0]->RenderMesh(GL_LINES);

		gridShader.setFloat("r", 0.0f);
		gridShader.setFloat("rg", 1.0f);
		gridShader.setFloat("rgb", 0.0f);
		objectList[5]->meshList[1]->RenderMesh(GL_LINES);

		gridShader.setFloat("r", 0.0f);
		gridShader.setFloat("rg", 0.0f);
		gridShader.setFloat("rgb", 1.0f);
		objectList[5]->meshList[2]->RenderMesh(GL_LINES);

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

	/////////////////////////////////////////
	// Creating Joel's name and ID object //
	////////////////////////////////////////
	glm::mat4 model = glm::mat4(1.0f);

	ComplexObject* letterJ = CreateLetterJ(modelLocation);
	ComplexObject* letterL = CreateLetterL(modelLocation);
	ComplexObject* joelNumber4 = CreateNumber4(modelLocation);
	ComplexObject* joelNumber5 = CreateLetterS(modelLocation); // Letter S is same model as number 5

	// Making the translations so that both letters can be side by side.
	// J translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, 0.1f, 0.0f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	letterJ->SetModelMatrix(model, modelLocation);

	// L translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.1f, -1.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	letterL->SetModelMatrix(model, modelLocation);

	// 4 translate
	// We also change its size to match other characters
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.8f, 0.5f, -1.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	joelNumber4->SetModelMatrix(model, modelLocation);

	// 5 translate
	// We also change its size to match other characters
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(6.5f, 0.0f, 0.28f));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.525f, 0.525f, 0.525f));
	joelNumber5->SetModelMatrix(model, modelLocation);

	ComplexObject* joelNameAndID = new ComplexObject();
	joelNameAndID->objectList.push_back(letterJ);
	joelNameAndID->objectList.push_back(letterL);
	joelNameAndID->objectList.push_back(joelNumber4);
	joelNameAndID->objectList.push_back(joelNumber5);

	objectList.push_back(joelNameAndID);

	//////////////////////////////////////////
	// Creating Razvan's name and ID object //
	//////////////////////////////////////////

	ComplexObject* letterR = CreateLetterR(modelLocation);
	ComplexObject* letterP = CreateLetterP(modelLocation);
	ComplexObject* number2 = CreateNumber2(modelLocation);
	ComplexObject* number4 = CreateNumber4(modelLocation);
	
	// R translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	letterR->SetModelMatrix(model, modelLocation);
	// P translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 0.0f, -2.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	letterP->SetModelMatrix(model, modelLocation);
	// 2 translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	number2->SetModelMatrix(model, modelLocation);
	// 4 translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.0f, 0.0f, -1.80f));
	model = glm::rotate(model, glm::radians(-18.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	number4->SetModelMatrix(model, modelLocation);

	ComplexObject* razvanNameAndID = new ComplexObject();
	razvanNameAndID->objectList.push_back(letterR);
	razvanNameAndID->objectList.push_back(letterP);
	razvanNameAndID->objectList.push_back(number2);
	razvanNameAndID->objectList.push_back(number4);

	objectList.push_back(razvanNameAndID);

	//////////////////////////////////////////
	// Creating Saffia's name and ID object //
	//////////////////////////////////////////

	ComplexObject* letterS = CreateLetterS(modelLocation);
	ComplexObject* letterA = CreateLetterA(modelLocation);
	ComplexObject* four = CreateNumber4alt(modelLocation);
	ComplexObject* number3 = CreateNumber3(modelLocation);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	letterS->SetModelMatrix(model, modelLocation);

   	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(6.5f, 0.0f, -2.5f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.98f, 1.0f));
	letterA->SetModelMatrix(model, modelLocation);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.0f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.99f, 1.0f));
	four->SetModelMatrix(model, modelLocation);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.5f, 0.0f, -4.5f));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	number3->SetModelMatrix(model, modelLocation);

	ComplexObject* SaffiaNameAndID = new ComplexObject();
	SaffiaNameAndID->objectList.push_back(letterS);
	SaffiaNameAndID->objectList.push_back(letterA);
	SaffiaNameAndID->objectList.push_back(four);
	SaffiaNameAndID->objectList.push_back(number3);

	objectList.push_back(SaffiaNameAndID);

    //////////////////////////////////////////
	// Creating Ian's name and ID object    //
	//////////////////////////////////////////

    ComplexObject* letterI = CreateLetterI(modelLocation);
    ComplexObject* letterN = CreateLetterN(modelLocation);
    ComplexObject* number6 = CreateNumber6(modelLocation);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	letterI->SetModelMatrix(model, modelLocation);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 0.0f, -2.5f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.98f, 1.0f));
	letterN->SetModelMatrix(model, modelLocation);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(9.5f, 0.0f, -3.5f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.95f, 1.0f));
	four->SetModelMatrix(model, modelLocation);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(20.5f, 0.0f, 0.5f));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	number6->SetModelMatrix(model, modelLocation);

	ComplexObject* IanNameAndID = new ComplexObject();
	IanNameAndID->objectList.push_back(letterI);
	IanNameAndID->objectList.push_back(letterN);
	IanNameAndID->objectList.push_back(four); 
	IanNameAndID->objectList.push_back(number6);

	objectList.push_back(IanNameAndID);

	//////////////////////////////////////////
	// Creating Michael's name and ID object    //
	//////////////////////////////////////////
	ComplexObject* letterM = CreateLetterM(modelLocation);
	ComplexObject* letterML = CreateLetterL(modelLocation);
	ComplexObject* number4Michael = CreateNumber4Michael(modelLocation);
	ComplexObject* number7 = CreateNumber7(modelLocation);


	// M translate //move the whole letter
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.4f, -10.5f, 2.4f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	letterM->SetModelMatrix(model, modelLocation);

	// L translate //move the whole letter
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(13.0f, -5.5f, -1.2f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	letterML->SetModelMatrix(model, modelLocation);

	//4 Translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(16.5f, -5.0f, -1.5f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	number4Michael->SetModelMatrix(model, modelLocation);

	//7 Translate
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(22.5f, -5.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	number7->SetModelMatrix(model, modelLocation);


	ComplexObject* michaelNameAndID = new ComplexObject();

	michaelNameAndID->objectList.push_back(letterM);
	michaelNameAndID->objectList.push_back(letterML);
	michaelNameAndID->objectList.push_back(number4Michael);
	michaelNameAndID->objectList.push_back(number7);


	objectList.push_back(michaelNameAndID);


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


ComplexObject* CreateLetterS(GLuint uniformModel)
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

	// LETTER S
	ComplexObject* s = new ComplexObject();

	// Creating the base 

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
ComplexObject* CreateNumber3(GLuint uniformModel)
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

	// NUMBER 3	
	ComplexObject * three = new ComplexObject();

	// First fragment
	glm::mat4 partModel(1.0f);
	partModel = glm::scale(partModel, glm::vec3(0.5f, 4.4f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(22.0f, 0.82f, 0.0f));
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

	return three;
}
ComplexObject* CreateNumber4alt (GLuint uniformModel)
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

	// NUMBER 4
	ComplexObject * four = new ComplexObject();

	// First fragment
	glm::mat4 partModel(1.0f);
	partModel = glm::scale(partModel, glm::vec3(0.5f, 4.45f, 1.0f));
	partModel = glm::translate(partModel, glm::vec3(11.0f, 0.8f, 0.0f));
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

	return four;
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

ComplexObject* CreateNumber6(GLuint uniformModel)
{

    ///////
    // 6 //
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
    ComplexObject *six = new ComplexObject();

    glm::mat4 partModel(1.0f);
    partModel = glm::scale(partModel, glm::vec3(2.75f, 0.5f, 1.0f));
    IndependentMesh *cube61 = new IndependentMesh();
    cube61->CreateMesh(vertices, indices, 24, 36);
    cube61->SetModelMatrix(partModel, uniformModel);
    six->meshList.push_back(cube61);

    partModel = glm::translate(partModel, glm::vec3(0.0f, 8.0f, 0.0f));
    IndependentMesh *cube62 = new IndependentMesh();
    cube62->CreateMesh(vertices, indices, 24, 36);
    cube62->SetModelMatrix(partModel, uniformModel);
    six->meshList.push_back(cube62);

    partModel = glm::translate(partModel, glm::vec3(0.0f, 6.0f, 0.0f));
    IndependentMesh *cube63 = new IndependentMesh();
    cube63->CreateMesh(vertices, indices, 24, 36);
    cube63->SetModelMatrix(partModel, uniformModel);
    six->meshList.push_back(cube63);

    partModel = glm::mat4(1.0f);
    partModel = glm::scale(partModel, glm::vec3(0.5f, 1.8f, 1.0f));
    partModel = glm::translate(partModel, glm::vec3(4.5f, 1.2f, -0.1f));
    IndependentMesh *cube64 = new IndependentMesh();
    cube64->CreateMesh(vertices, indices, 24, 36);
    cube64->SetModelMatrix(partModel, uniformModel);
    six->meshList.push_back(cube64);

    partModel = glm::translate(partModel, glm::vec3(-9.1f, 0.9f, -0.1f));
    partModel = glm::scale(partModel, glm::vec3(1.0f, 2.0f, 1.0f));
    IndependentMesh *cube65 = new IndependentMesh();
    cube65->CreateMesh(vertices, indices, 24, 36);
    cube65->SetModelMatrix(partModel, uniformModel);
    six->meshList.push_back(cube65);

    return six;
}

ComplexObject* CreateLetterM(GLuint uniformModel)
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

	// LETTER M
	glm::mat4 model = glm::mat4(1.0f);

	// Creating the base of the letter L
	IndependentMesh* lBase = new IndependentMesh();
	lBase->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(3.0f, 9.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.0f, 1.0f, 1.0f));

	lBase->SetModelMatrix(model, uniformModel);

	IndependentMesh* mLeftPillar = new IndependentMesh();
	mLeftPillar->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.5f, 7.0f, 0.0f));//where
	model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));//size

	mLeftPillar->SetModelMatrix(model, uniformModel);

	IndependentMesh* mRightPillar = new IndependentMesh();
	mRightPillar->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(5.5f, 7.0f, 0.0f));//where
	model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));//size

	mRightPillar->SetModelMatrix(model, uniformModel);

	IndependentMesh* mMidPillar = new IndependentMesh();
	mMidPillar->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(3.0f, 7.5f, 0.0f));//where
	model = glm::scale(model, glm::vec3(1.0f, 2.5f, 1.0f));//size

	mMidPillar->SetModelMatrix(model, uniformModel);

	ComplexObject* mLetter = new ComplexObject();
	mLetter->meshList.push_back(lBase);
	mLetter->meshList.push_back(mLeftPillar);
	mLetter->meshList.push_back(mRightPillar);
	mLetter->meshList.push_back(mMidPillar);

	return mLetter;
}
ComplexObject* CreateNumber4Michael(GLuint uniformModel)
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

	// Creating the base of the Number 4
	IndependentMesh* lBase = new IndependentMesh();
	lBase->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.5f, 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 1.0f, 1.0f));

	lBase->SetModelMatrix(model, uniformModel);

	// Creating the trunk of the Number 4
	IndependentMesh* lTrunk = new IndependentMesh();
	lTrunk->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(2.0f, 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 6.0f, 1.0f));

	lTrunk->SetModelMatrix(model, uniformModel);

	IndependentMesh* top = new IndependentMesh();
	top->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(-1.0f, 3.5f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 2.5f, 1.0f));

	top->SetModelMatrix(model, uniformModel);

	ComplexObject* number4 = new ComplexObject();
	number4->meshList.push_back(lBase);
	number4->meshList.push_back(lTrunk);
	number4->meshList.push_back(top);

	return number4;
}

ComplexObject* CreateNumber7(GLuint uniformModel)
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

	// Number 7
	glm::mat4 model = glm::mat4(1.0f);

	// Creating the base of the Number 7
	IndependentMesh* lBase = new IndependentMesh();
	lBase->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.5f, 4.25f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 1.0f, 1.0f));

	lBase->SetModelMatrix(model, uniformModel);

	// Creating the trunk of the letter L
	IndependentMesh* left = new IndependentMesh();
	left->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

	left->SetModelMatrix(model, uniformModel);

	IndependentMesh* box = new IndependentMesh();
	box->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(1.0f, 1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	box->SetModelMatrix(model, uniformModel);

	IndependentMesh* right = new IndependentMesh();
	right->CreateMesh(vertices, indices, 24, 36);
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(2.0f, 3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

	right->SetModelMatrix(model, uniformModel);

	ComplexObject* number7 = new ComplexObject();
	number7->meshList.push_back(lBase);
	number7->meshList.push_back(left);
	number7->meshList.push_back(box);
	number7->meshList.push_back(right);

	return number7;
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
