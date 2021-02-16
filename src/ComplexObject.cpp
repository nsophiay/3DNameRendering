#include "ComplexObject.h"

ComplexObject::ComplexObject()
{
	meshList = std::vector<Mesh*>();
	uniformObjectModelLocation = 0;
	objectModelMatrix = new glm::mat4(1.0f);

	objectList = std::vector<ComplexObject*>();

	hasModelMatrix = false;
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

void ComplexObject::RenderObject(glm::mat4& modelMatrix, GLuint uniformModel)
{
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
		objectList[i]->RenderObject(model, uniformModel);
	}
	
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
