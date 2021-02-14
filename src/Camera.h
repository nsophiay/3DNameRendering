#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed);
	
	void movementFromKeyboard(bool* keys); // WASD movement
	void movementFromKeyboard(bool* keys, GLfloat deltatime); // WASD movement using delta time
	void pan(bool* keys,  GLfloat x); // Pan while holding down right button
	void tilt(bool* keys, GLfloat y); // Tilt while holding down down button
	glm::mat4 calculateViewMatrix(); // Get the view matrix
	glm::mat4 calculateViewMatrix(glm::vec3 eye, glm::vec3 target, glm::vec3 up);
	
	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw; // Side to side
	GLfloat pitch; // Up and down

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();
};