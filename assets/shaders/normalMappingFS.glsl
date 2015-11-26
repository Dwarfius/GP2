#version 330

layout(location = 0) out vec4 FragColor;

in vec2 vertexTexCoordsOut;
in mat3 TBN;

uniform vec3 lightDirection;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	vec3 normal = texture(texture1, vertexTexCoordsOut).rgb;
	normal = normalize(normal * 2.0 - 1.0);   
	normal = normalize(TBN * normal); 
	float diffuseTerm = dot(normal, lightDirection);

	FragColor = texture(texture0, vertexTexCoordsOut) * diffuseTerm;
}
