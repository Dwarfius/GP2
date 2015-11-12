#version 150

//The blending range
#define EPSILON 0.1
//The step for each component
#define T 0.4

in float height;
in vec2 uvsOut;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main()
{
	float k0 = 1 - height / (T + EPSILON);
	k0 = clamp(k0, 0, 1);
	float k1 = 1 - abs(height - 0.5) / (0.5 - T + EPSILON);
	k1 = clamp(k1, 0, 1);
	float k2 = (height - (1 - T - EPSILON)) / (T + EPSILON);
	k2 = clamp(k2, 0, 1);
	FragColor = texture(texture0, uvsOut) * k0 + texture(texture1, uvsOut) * k1 + texture(texture2, uvsOut) * k2;
	//FragColor = vec4(k0, k1, k2, 1);
}