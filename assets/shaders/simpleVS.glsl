#version 150

in vec3 vertexPos;
in vec4 color;

uniform mat4 MVP;

out vec4 colorOut;

void main()
{
	gl_Position = MVP * vec4(vertexPos, 1);
	colorOut = color;
}