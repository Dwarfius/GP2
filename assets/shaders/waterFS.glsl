#version 330

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

in vec2 vertexTexCoordsOut;
in vec3 tangentEye;
in vec3 tangentPos;

uniform vec3 lightDirection;
uniform sampler2D texture0; //albedo
uniform sampler2D texture1; //normal map
uniform sampler2D texture2; //heightmap

uniform float heightScale;
uniform float timer;
  
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float height = texture(texture2, texCoords).r;    
    vec2 p = viewDir.xy / viewDir.z * (height);// * heightScale);
    return texCoords - p;    
} 

void main()
{
	//vec2 newVertexTexCoordsOut = vec2(vertexTexCoordsOut.x, vertexTexCoordsOut.y + timer);
	vec3 viewDir = normalize(tangentEye - tangentPos);
    vec2 texCoords = ParallaxMapping(vertexTexCoordsOut, viewDir);
	
	//vec3 worldNormal = texture(texture1, texCoords).rgb;
	vec3 worldNormal = texture(texture1, vertexTexCoordsOut).rgb;
	worldNormal = normalize(worldNormal * 2.0 - 1.0);

	FragColor = texture(texture0, vertexTexCoordsOut);
	Normal = worldNormal;
}
