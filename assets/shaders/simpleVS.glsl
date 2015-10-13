#version 150

in vec3 vertexPos;
in vec2 uvs;

uniform mat4 MVP;

out vec2 uvsOut;

void main()
{
	gl_Position = MVP * vec4(vertexPos, 1);
	uvsOut = uvs;
}