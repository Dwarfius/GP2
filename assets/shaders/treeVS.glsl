#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 colors;
layout(location = 2) in vec2 uvs;
layout(location = 3) in vec3 normals;

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