#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

in vec2 vertexTexCoordsOut;
in mat3 TBN;
in vec3 tangentViewPos;
in vec3 tangentFragPos;

uniform vec3 lightDirection;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
  
uniform float heightScale;
uniform float timer;
  
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float height = texture(texture2, texCoords).r;    
    vec2 p = viewDir.xy / viewDir.z * (height * heightScale);
    return texCoords - p.xy;    
} 

void main()
{
	vec2 newVertexTexCoordsOut = vec2(vertexTexCoordsOut.x, vertexTexCoordsOut.y + timer);
    vec3 viewDir   = normalize(tangentViewPos - tangentFragPos);
    vec2 texCoords = ParallaxMapping(newVertexTexCoordsOut,  viewDir);
	
	vec3 worldNormal = texture(texture1, texCoords).rgb;
	worldNormal = normalize(worldNormal * 2.0 - 1.0);   
	worldNormal = normalize(TBN * worldNormal); 

	FragColor = texture(texture0, vec2(texCoords.x, texCoords.y + timer/2));
	//FragColor = vec4(worldNormal, 0.0);
	Normal = worldNormal;
}
