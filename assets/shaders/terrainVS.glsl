#version 330

in vec3 vertexPosition;
in vec2 uvs;
in vec4 colors;
in vec3 normals;

uniform mat4 MVP;

out vec2 uvsOut;
out vec4 ranges;
out vec3 normal;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1);
	uvsOut = uvs;
	ranges = colors;
	normal = (MVP * vec4(normals, 0)).xyz;
}