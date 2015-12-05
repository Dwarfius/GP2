#version 330

flat in int textId;
in vec2 uvsOut;
in vec3 normal;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 Normal;

void main()
{
	switch(textId)
	{
		case 0:
		{
			FragColor = texture(texture0, uvsOut);
			//vec4 texelColor = texture(texture0, uvsOut);
			//FragColor = vec4(1, 0, 0, 1);
		}
			break;
		case 1:
		{
			FragColor = texture(texture1, uvsOut);
			//vec4 texelColor = texture(texture1, uvsOut);
			//FragColor = vec4(0, 1, 0, 1);
		}
			break;
		case 2:
		{
			vec4 texelColor = texture(texture2, uvsOut);
			if(texelColor.a < 0.5)
				discard;
			FragColor = texelColor;
			//FragColor = vec4(0, 0, 1, 1);
		}
			break;
	}
	Normal = normal * 0.5 + 0.5;
}