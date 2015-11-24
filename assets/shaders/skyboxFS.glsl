#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
	FragColor = texture(skybox, TexCoords);
	Normal = vec3(0.5, 0.5, 0.5);
}