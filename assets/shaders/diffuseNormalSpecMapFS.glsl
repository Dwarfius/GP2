#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

in vec2 vertexTexCoordsOut;
in mat3 TBN;
in vec3 cameraDirectionOut;
in vec4 FragPosLightSpace;

uniform vec3 lightDirection;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D shadowMap;

uniform float specularPower;
uniform vec4 specularLightColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}  

void main()
{
	vec3 worldNormal = texture(texture1, vertexTexCoordsOut).rgb;
	worldNormal = normalize(worldNormal * 2.0 - 1.0);   
	worldNormal = normalize(TBN * worldNormal); 
	vec3 halfWayVec = normalize(cameraDirectionOut + lightDirection);
	float specularTerm = pow(clamp(dot(worldNormal, halfWayVec), 0.f, 1.f), specularPower);
	vec4 color = texture(texture0, vertexTexCoordsOut);
	float shadow = ShadowCalculation(FragPosLightSpace);
	
	FragColor = (1.0 - shadow)*(color + (texture(texture2, vertexTexCoordsOut) * specularLightColor * specularTerm));
	//FragColor = vec4(worldNormal, 0.0);
	Normal = worldNormal * 0.5 + 0.5;
}
