#version 150

in vec2 uvsOut;

uniform sampler2D texture0;

out vec4 FragColor;

void main()
{
	FragColor = vec4(1, 1, 1, 1);
	//FragColor = vec4(1, 1, 1, 1) - texture(texture0, uvsOut);
}