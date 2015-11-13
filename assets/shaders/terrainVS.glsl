#version 150

in vec3 vertexPosition;
in vec2 uvs;
in vec4 colors;

uniform mat4 MVP;

out vec2 uvsOut;
out vec4 ranges;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1);
	uvsOut = uvs;
	ranges = colors;
}