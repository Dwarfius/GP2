#version 330

in vec4 ranges;
in vec2 uvsOut;
in vec3 normal;
in vec4 FragPosLightSpace;


uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D shadowMap;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

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
	float shadow = ShadowCalculation(FragPosLightSpace);
	FragColor = (1.0 - shadow)*(texture(texture0, uvsOut) * ranges.x + texture(texture1, uvsOut) * ranges.y + texture(texture2, uvsOut) * ranges.z);
	Normal = normal * 0.5 + 0.5;
}