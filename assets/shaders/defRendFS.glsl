#version 330

in vec2 vertexUV;

uniform sampler2D texture0; //color
uniform sampler2D texture1; //normals
uniform sampler2D texture2; //depth

uniform vec3 sunDir;
uniform vec4 sunColor;

out vec4 FragColor;

//The goal of this shader is to join up all the textures in to the final image(color/normal/depth)
void main()
{
	vec4 color = texture(texture0, vertexUV);
	vec3 normal = texture(texture1, vertexUV).xyz * 2 - 1;
	float coeff = -dot(normal, sunDir);
	
	//FragColor = vec4(0, color.y, 0, 1) * 2 - 1;
	//FragColor = abs(vec4(normal, 1));
	//FragColor = vec4(0, normal.y, 0, 1) * 2 - 1;
	//FragColor = vec4(coeff, coeff, coeff, 1);
	//FragColor = vec4(-sunDir, 1);
	FragColor = color + coeff * sunColor;
}