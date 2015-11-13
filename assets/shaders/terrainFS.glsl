#version 150

in vec4 ranges;
in vec2 uvsOut;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture0, uvsOut) * ranges.x + 
			texture(texture1, uvsOut) * ranges.y + 
			texture(texture2, uvsOut) * ranges.z;
	//FragColor = vec4(k0, k1, k2, 1);
}