#version 330

in vec3 vertexPosition;
in vec4 colors;
in vec2 uvs;
in vec3 normals;

uniform mat4 MVP;

out vec2 uvsOut;
flat out int textId;
out vec3 normal;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1);
	uvsOut = uvs;
	textId = int(colors.x);
	normal = (MVP * vec4(normals, 0)).xyz;
}