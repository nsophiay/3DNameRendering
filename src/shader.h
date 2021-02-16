#pragma once

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/* Entire process of creating a vertex and fragment shader from source code on disk, compiling them and then creating and linking a program*/
class Shader
{
public:
	unsigned int ID;

	// Constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void free(); // free program
	unsigned int getId();

	// Utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMatrix4Float(const std::string& name, glm::mat4* transformMatrix) const;
	GLuint getLocation(const std::string& name) const;

};