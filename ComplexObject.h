#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>
#include <GLFW/glfw3.h>

class ComplexObject
{
	public:
		/// <summary>
		/// Creates a ComplexObject, an object capable of containing many meshes and many other complex objects. 
		/// It can also have its own transformations, held in a model matrix.
		/// </summary>
		ComplexObject();
		~ComplexObject();

		/// <summary>
		/// Renders the complex object on screen.
		/// </summary>
		void RenderObject();
		
		/// <summary>
		/// Renders the complex object on screen, applying the specified Model Matrix to it.
		/// </summary>
		/// <param name="modelMatrix">The model matrix value.</param>
		/// <param name="uniformModel">The location of the uniform variable the Model Matrix is tied to.</param>
		void RenderObject(glm::mat4& modelMatrix, GLuint uniformModel);

		/// <summary>
		/// Renders the complex object on screen, applying the specified Model Matrix to it.
		/// </summary>
		/// <param name="shader">The chosen shader.</param>
		void RenderObject(Shader shader);

		/// <summary>
		/// Renders the complex object on screen, applying the specified Model Matrix to it.
		/// </summary>
		/// <param name="modelMatrix">The model matrix value.</param>
		/// <param name="uniformModel">The location of the uniform variable the Model Matrix is tied to.</param>
		/// <param name="shader">The chosen shader.</param>
		void RenderObject(glm::mat4& modelMatrix, GLuint uniformModel, Shader shader);

		/// <summary>
		/// Clears the object from the GPU.
		/// </summary>
		void ClearObject();

		/// <summary>
		/// The list of meshes inside this object.
		/// </summary>
		std::vector<Mesh*> meshList;
		/// <summary>
		/// The list of other complex objects inside this object.
		/// </summary>
		std::vector<ComplexObject*> objectList;

		/// <summary>
		/// Sets the model matrix of this object, to apply custom transformations to the entire object.
		/// </summary>
		/// <param name="matrix">The model matrix value.</param>
		/// <param name="uniformModelLocation">The location of the uniform variable the Model Matrix is tied to.</param>
		void SetModelMatrix(glm::mat4& matrix, GLuint uniformModelLocation);

		/// <summary>
		/// Cleans up the Model matrix of this object, so that it no longer applies custom transformations.
		/// </summary>
		void ResetModelMatrix();

		/// <summary>
		/// Returns the current model matrix tied to this object.
		/// </summary>
		/// <returns>A reference to the mat4 of values corresponding to the model matrix</returns>
		glm::mat4& GetModelMatrix();


		/// <summary>
		/// Sets the colour of the object.
		/// </summary>
		/// <param name="r">Sets the red value.</param>
		/// <param name="g">Sets the green value.</param>
		/// <param name="b">Sets the blue value.</param>
		void SetColour(GLfloat r, GLfloat g, GLfloat b);

		/// <summary>
		/// Sets the colour of the object.
		/// </summary>
		/// <param name="hex">Sets the RGB value from a hex colour.</param>
		void SetColour(int hex);

		/// <summary>
		/// Sets the colour of the object.
		/// </summary>
		/// <param name="hex">Sets the RGB value from a hex colour.</param>
		void SetTexture(Texture tex, GLuint textureLocation);

        /// <summary>
        // Translates model.
        // </summary>
        // <param name="x">Amount to translate in the x direction.</param>
        // <param name="y">Amount to translate in the y direction.</param>
        // <param name="z">Amount to translate in the z direction.</param>
        void TranslateModel(GLfloat x, GLfloat y, GLfloat z);

        /// <summary>
        // Rotates model.
        // </summary>
        // <param name="x">x component of axis of rotation.</param>
        // <param name="y">y component of axis of rotation.</param>
        // <param name="z">z component of axis of rotation.</param>
        // <param name="angle">Amount to rotate model in degrees.</param>
        void RotateModel(GLfloat x, GLfloat y, GLfloat z, GLfloat angle);

        /// <summary>
        // Scales model.
        // </summary>
        // <param name="xScale">Amount to scale in the x direction.</param>
        // <param name="yScale">Amount to scale in the y direction.</param>
        // <param name="zScale">Amount to scale in the z direction.</param>
        void ScaleModel(GLfloat xScale, GLfloat yScale, GLfloat zScale);

        /// <summary>
        // Transforms model based on keyboard input
        // </summary>
        // <param name="keys">Array of pressed keys.</param>
        void Transform(bool* keys);

		/// <summary>
		/// Converts a hex colour code into RGB
		/// </summary>
		/// <param name="hexValue">The colour in hex.</param>
		/// <returns>An array of 3 floats representing the red, green, and blue values.</returns>
		float* hexToRGB(int hexValue);

	private:
		/// <summary>
		/// The model matrix of this object.
		/// </summary>
		glm::mat4* objectModelMatrix;
		/// <summary>
		/// The location of the uniform variable tied to this object's model matrix.
		/// </summary>
		GLuint uniformObjectModelLocation;
		GLuint uniformTextureLocation;
		/// <summary>
		/// Determines if this object has a model matrix tied to it currently. True if yes, false otherwise.
		/// </summary>
		bool hasModelMatrix;

		GLfloat red, green, blue;
		float initialR, initialG, initialB;
		bool colourHasBeenSet, textureHasBeenSet;

		Texture tex;
};

