#version 330 core					

out vec4 FragColor;			

in vec3 vertexColor;			

uniform float redColor;
uniform float greenColor;
uniform float blueColor;

void main()				
{					
	FragColor = vec4(vertexColor, 1.0);					
}														