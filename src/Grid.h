#pragma once

#include <iostream>
#include <vector>

class Grid
{
private:
	std::vector<float> vertices;			
	std::vector<unsigned int> indices;
	int squareCount;
	unsigned int indexCount;

	void createVertices();
	void createIndices();
public:
	Grid();
	Grid(int squareCount);
	~Grid();
	
	unsigned int getIndexCount();
	std::vector<float> getVertexArr();
	std::vector<unsigned int> getIndexArr();

};

