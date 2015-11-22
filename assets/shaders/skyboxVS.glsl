#version 150
in vec3 vertexPosition;
out vec3 TexCoords;

uniform mat4 MVP;

void main()
{
    gl_Position =   MVP * vec4(vertexPosition, 1.0);  
    TexCoords = vertexPosition;
}  