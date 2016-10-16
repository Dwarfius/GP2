#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

in vec2 vertexTexCoordsOut;
in mat3 TBN;
in vec3 cameraDirectionOut;

uniform vec3 lightDirection;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float specularPower;
uniform vec4 specularLightColor;

void main()
{
	vec3 worldNormal = texture(texture1, vertexTexCoordsOut).rgb;
	worldNormal = normalize(worldNormal * 2.0 - 1.0);   
	worldNormal = normalize(TBN * worldNormal); 
	vec3 halfWayVec = normalize(cameraDirectionOut + lightDirection);
	float specularTerm = pow(clamp(dot(worldNormal, halfWayVec), 0.f, 1.f), specularPower);

	FragColor = texture(texture0, vertexTexCoordsOut) + (texture(texture2, vertexTexCoordsOut) * specularLightColor * specularTerm);
	Normal = worldNormal;
}
