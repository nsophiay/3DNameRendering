#include "ComplexObject.h"


ComplexObject::ComplexObject()
{
	meshList = std::vector<Mesh*>();
	uniformObjectModelLocation = 0;
	objectModelMatrix = new glm::mat4(1.0f);

	objectList = std::vector<ComplexObject*>();

	hasModelMatrix = false;

	// Default colour is grey
	red = 0.55f;
	green = 0.55f;
	blue = 0.55f;

	textureHasBeenSet = false;
	colourHasBeenSet = false;
}

ComplexObject::~ComplexObject()
{
	// Clears the objectlist
	for (int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->ClearObject();
	}

	// Clean up the meshlist.
	for (int i = 0; i < meshList.size(); i++)
	{
		meshList[i]->~Mesh();
		delete meshList[i];
	}

	if (hasModelMatrix)
	{
		// If we have set the model matrix, clean it up.
		ResetModelMatrix();
	}
	else
	{
		// If not, just clean the pointer.
		delete objectModelMatrix;
		objectModelMatrix = NULL;
	}

	// Destroy the object list.
	for (int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->~ComplexObject();
		delete objectList[i];
	}
}

void ComplexObject::RenderObject()
{
	// If we have a custom transformation...
	if (hasModelMatrix)
	{
		// ... we apply it to our children, rendering them with it.
		for (int i = 0; i < meshList.size(); i++)
		{
			meshList[i]->RenderMesh(*objectModelMatrix, uniformObjectModelLocation);
		}

		for (int i = 0; i < objectList.size(); i++)
		{
			objectList[i]->RenderObject(*objectModelMatrix, uniformObjectModelLocation);
		}
	}
	else
	{
		// No transformation means we just render our children as is.
		for (int i = 0; i < meshList.size(); i++)
		{
			meshList[i]->RenderMesh();
		}

		for (int i = 0; i < objectList.size(); i++)
		{
			objectList[i]->RenderObject();
		}
	}
}

void ComplexObject::RenderObject(Shader shader)
{
	shader.setFloat("r", red); // Red
	shader.setFloat("rg", green); // Green
	shader.setFloat("rgb", blue); // Blue

	if (textureHasBeenSet) {
		tex.loadTexture();
		tex.useTexture();
		glUniform1i(uniformTextureLocation, 0);
	}

	// If we have a custom transformation...
	if (hasModelMatrix)
	{
		// ... we apply it to our children, rendering them with it.
		for (int i = 0; i < meshList.size(); i++)
		{
			meshList[i]->RenderMesh(*objectModelMatrix, uniformObjectModelLocation);
		}

		for (int i = 0; i < objectList.size(); i++)
		{
			objectList[i]->RenderObject(*objectModelMatrix, uniformObjectModelLocation, shader);
		}
	}
	else
	{
		// No transformation means we just render our children as is.
		for (int i = 0; i < meshList.size(); i++)
		{
			meshList[i]->RenderMesh();
		}

		for (int i = 0; i < objectList.size(); i++)
		{
			objectList[i]->RenderObject(shader);
		}
	}
}



void ComplexObject::RenderObject(glm::mat4& modelMatrix, GLuint uniformModel)
{
	glm::mat4 model(1.0f);

	if (textureHasBeenSet) {
		tex.loadTexture();
		tex.useTexture();
		glUniform1i(uniformTextureLocation, 0);
	}

	if (hasModelMatrix)
	{
		// If we have a custom transformation, we combine it with the provided transformation.
		model = modelMatrix * *objectModelMatrix;
	}
	else
	{
		// If we dont have a current transformation, just pass the provided transformation.
		model = modelMatrix;
	}

	// Rendering our children
	for (int i = 0; i < meshList.size(); i++)
	{
		meshList[i]->RenderMesh(model, uniformModel);
	}

	for (int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->RenderObject(model, uniformModel);
	}
	
}

void ComplexObject::RenderObject(glm::mat4& modelMatrix, GLuint uniformModel, Shader shader)
{
	shader.setFloat("r", red); // Red
	shader.setFloat("rg", green); // Green
	shader.setFloat("rgb", blue); // Blue

	if (textureHasBeenSet) {
		tex.loadTexture();
		tex.useTexture();
		glUniform1i(uniformTextureLocation, 0);
	}

	glm::mat4 model(1.0f);

	if (hasModelMatrix)
	{
		// If we have a custom transformation, we combine it with the provided transformation.
		model = modelMatrix * *objectModelMatrix;
	}
	else
	{
		// If we dont have a current transformation, just pass the provided transformation.
		model = modelMatrix;
	}

	// Rendering our children
	for (int i = 0; i < meshList.size(); i++)
	{
		meshList[i]->RenderMesh(model, uniformModel);
	}

	for (int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->RenderObject(model, uniformModel, shader);
	}

}



void ComplexObject::SetColour(GLfloat r, GLfloat g, GLfloat b) {

	red = r;
	green = g;
	blue = b;

	if (!colourHasBeenSet) {
		initialR = red;
		initialG = green;
		initialB = blue;
		colourHasBeenSet = true;
	}

}

void ComplexObject::SetColour(int hex) {

	float* rgb = hexToRGB(hex);
	red = rgb[0];
	green = rgb[1];
	blue = rgb[2];

	if (!colourHasBeenSet) {
		initialR = red;
		initialG = green;
		initialB = blue;
		colourHasBeenSet = true;
	}
}

void ComplexObject::SetTexture(Texture tex, GLuint textureLocation) {
	this->tex = tex;
	uniformTextureLocation = textureLocation;
	textureHasBeenSet = true;
}

void ComplexObject::ClearObject()
{
	// Clears the meshlist
	for (int i = 0; i < meshList.size(); i++)
	{
		meshList[i]->ClearMesh();
	}

	// Clears the objectlist
	for (int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->ClearObject();
	}	
}

void ComplexObject::SetModelMatrix(glm::mat4& matrix, GLuint uniformModelLocation)
{
	if (hasModelMatrix) {
		// Removing the model matrix from gpu
		glUniform1f((*this).uniformObjectModelLocation, 0.0f);
	}

	*objectModelMatrix = matrix;
	uniformObjectModelLocation = uniformModelLocation;

	hasModelMatrix = true;
}

void ComplexObject::ResetModelMatrix()
{
	hasModelMatrix = false;

	// Removing the model matrix from gpu
	glUniform1f((*this).uniformObjectModelLocation, 0.0f);

	uniformObjectModelLocation = 0;

	delete objectModelMatrix;
	objectModelMatrix = NULL;
}

glm::mat4& ComplexObject::GetModelMatrix()
{
	if (hasModelMatrix)
		return *objectModelMatrix;
	else
	{
		// If no matrix has been set for this object, we return a generic identity matrix.
		// Could throw an exception too, because if someone tries to modify that generic matrix, it wont change this object's matrix.
		glm::mat4 model = glm::mat4(1.0f);
		return model;
	}
}

void ComplexObject::TranslateModel(GLfloat x, GLfloat y, GLfloat z)
{
    glm::mat4 model = GetModelMatrix();
    model = glm::translate(model, glm::vec3(x, y, y));
    SetModelMatrix(model, uniformObjectModelLocation);
}

void ComplexObject::RotateModel(GLfloat x, GLfloat y, GLfloat z, GLfloat angle){
    glm::mat4 model = GetModelMatrix();
    model = glm::rotate(model, angle, glm::vec3(x, y, z));
    SetModelMatrix(model, uniformObjectModelLocation);
}

void ComplexObject::ScaleModel(GLfloat xScale, GLfloat yScale, GLfloat zScale)
{
    glm::mat4 model = GetModelMatrix();
    model = glm::scale(model, glm::vec3(xScale, yScale, zScale));
    SetModelMatrix(model, uniformObjectModelLocation);
}

void ComplexObject::Transform(bool* keys)
{

    // Move up when capital W is pressed
    if(keys[GLFW_KEY_W] && keys[GLFW_KEY_LEFT_SHIFT])
    {
        TranslateModel(0.0f, 0.05f, 0.0f);
    }
    // Move left when capital A is pressed
    if(keys[GLFW_KEY_A] && keys[GLFW_KEY_LEFT_SHIFT])
    {
        TranslateModel(-0.05f, 0.0f, 0.0f);
    }
    // Move down when capital S is pressed
    if(keys[GLFW_KEY_S] && keys[GLFW_KEY_LEFT_SHIFT])
    {
        TranslateModel(0.0f, -0.05f, 0.0f);
    }
    // Move right when capital D is pressed
    if(keys[GLFW_KEY_D] && keys[GLFW_KEY_LEFT_SHIFT])
    {
        TranslateModel(0.05f, 0.0f, 0.0f);
    }
    // Rotate Model 5deg right
    if(keys[GLFW_KEY_D] && !keys[GLFW_KEY_LEFT_SHIFT])
    {
        RotateModel(0.0f, 1.0f, 0.0f, (5.0f*3.14159f)/180.0f);
    }
    // Rotate Model 5deg left
    if(keys[GLFW_KEY_A] && !keys[GLFW_KEY_LEFT_SHIFT])
    {
        RotateModel(0.0f, 1.0f, 0.0f, -(5.0f*3.14159f)/180.0f);
    }
    // Scale up with U is pressed
    if(keys[GLFW_KEY_U]){
        ScaleModel(1.01f, 1.01f, 1.01f);
    }
    // Scale down with J is pressed
    if(keys[GLFW_KEY_J] && !keys[GLFW_KEY_LEFT_SHIFT]){
        ScaleModel(0.99f, 0.99f, 0.99f);
    }

	// Adjust red
	if (keys[GLFW_KEY_7]) {
		if(keys[GLFW_KEY_KP_ADD]) red += 0.08;
		else if (keys[GLFW_KEY_KP_SUBTRACT]) red -= 0.08;
	}

	// Adjust green
	if (keys[GLFW_KEY_8]) {
		if (keys[GLFW_KEY_KP_ADD]) green += 0.08;
		else if (keys[GLFW_KEY_KP_SUBTRACT]) green -= 0.08;
	}

	// Adjust blue
	if (keys[GLFW_KEY_9]) {
		if (keys[GLFW_KEY_KP_ADD]) blue += 0.08;
		else if (keys[GLFW_KEY_KP_SUBTRACT]) blue -= 0.08;
	}

	// Reset colour
	if (keys[GLFW_KEY_0]) {
		red = initialR;
		green = initialG;
		blue = initialB;
	}
}

float* ComplexObject::hexToRGB(int hexValue) {

	float rgb[3];

	/////////////////////////////////////////////////////
	//// Source: https://gist.github.com/bert/998020 ////

	rgb[0] = ((hexValue >> 16) & 0xFF) / 255.0; // Extract the RR byte
	rgb[1] = ((hexValue >> 8) & 0xFF) / 255.0; // Extract the GG byte
	rgb[2] = ((hexValue) & 0xFF) / 255.0; // Extract the BB byte

	/////////////////////////////////////////////////////

	return rgb;
}
