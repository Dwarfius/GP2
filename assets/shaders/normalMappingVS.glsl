#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoords;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec3 vertexBinormal;

out vec2 vertexTexCoordsOut;
out mat3 TBN;

uniform mat4 MVP;
uniform mat4 Model;

void main()
{
	vec3 T = normalize(vec3(Model * vec4(vertexTangent, 0.0)));
	vec3 N = normalize(vec3(Model * vec4(vertexNormal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(T, N);

	TBN = mat3(T, B, N);  
	vertexTexCoordsOut = vertexTexCoords;
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}