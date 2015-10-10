#version 150

in vec4 colorOut;

uniform vec3 colorIn;

out vec4 FragColor;

void main()
{
	FragColor = vec4(colorIn, 1) * colorOut;
}