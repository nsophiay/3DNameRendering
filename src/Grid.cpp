#include "Grid.h"

Grid::Grid()
{
	squareCount = 0;
	indexCount = 0;
}

Grid::Grid(int squareCount)
{
	vertices.clear();
	indices.clear();
	this->squareCount = squareCount; 
	
	createVertices();
	createIndices();
}

void Grid::createVertices()
{
	// Creates x, y, z coords (between 0 and 1) for vertices on each row
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
}

void Grid::createIndices()
{
	// Creates pairs of indices to to connect all points using GL_LINES
	// Each square gets drawn in a clockwise motion (top, right side, bottom, left side)
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

	indexCount = (unsigned int)indices.size();
}

unsigned int Grid::getIndexCount()
{
	return indexCount;
}

std::vector<float> Grid::getVertexArr()
{
	return vertices;
}

std::vector<unsigned int> Grid::getIndexArr()
{
	return indices;
}

Grid::~Grid()
{
	vertices.clear();
	indices.clear();
}