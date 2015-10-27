#version 150

out vec4 FragColor;

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
	float specularTerm = pow(dot(worldNormal, halfWayVec), specularPower);

	FragColor = (ambientMaterialColor*ambientLightColor) + (diffuseMaterialColor*diffuseLightColor*diffuseTerm) + (specularMaterialColor*specularLightColor*specularTerm);
	//FragColor = vec4(worldNormal, 1);
}
