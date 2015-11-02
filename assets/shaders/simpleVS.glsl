#version 150

in vec3 vertexPosition;
in vec2 uvs;

uniform mat4 MVP;

out vec2 uvsOut;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1);
	uvsOut = uvs;
}