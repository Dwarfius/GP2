#version 150
in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;
uniform samplerCube skyboxNight;
uniform float blendFactor;

void main()
{    
    FragColor = mix(texture(skybox, TexCoords), texture(skyboxNight, TexCoords), blendFactor);
}