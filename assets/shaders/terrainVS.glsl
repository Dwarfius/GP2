#version 150

#define HEIGHT 4.0

in vec3 vertexPosition;
in vec2 uvs;

uniform mat4 MVP;

out vec2 uvsOut;
out float height;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1);
	uvsOut = uvs;
	height = vertexPosition.y / HEIGHT;
}