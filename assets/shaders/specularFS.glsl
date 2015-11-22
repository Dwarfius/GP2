#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

in vec3 worldNormal;
in vec3 cameraDirectionOut;

uniform vec3 lightDirection;

uniform vec4 ambientMaterialColor;
uniform vec4 diffuseMaterialColor;
uniform vec4 specularMaterialColor;
uniform float specularPower;

uniform vec4 ambientLightColor;
uniform vec4 diffuseLightColor;
uniform vec4 specularLightColor;

void main()
{
	float diffuseTerm = dot(worldNormal, lightDirection);
	vec3 halfWayVec = normalize(cameraDirectionOut + lightDirection);
	float specularTerm = pow(clamp(dot(worldNormal, halfWayVec), 0.f, 1.f), specularPower);

	FragColor = (ambientMaterialColor*ambientLightColor) + 
		(diffuseMaterialColor*diffuseLightColor*diffuseTerm) +
		(specularMaterialColor*specularLightColor*specularTerm);
	Normal = worldNormal * 0.5 + 0.5;
}
