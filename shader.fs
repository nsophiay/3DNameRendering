#version 330

out vec4 FragColor;			

in vec3 vertexColor;
in vec2 texCoord;

struct DirectionalLight 
{
	vec3 colour;
	float ambientIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight dl;

void main()				
{					
    vec4 ambientColour = vec4(dl.colour, 1.0f) * dl.ambientIntensity;
    FragColor = texture(theTexture, texCoord);
}														