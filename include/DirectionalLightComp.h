#ifndef _DIRLIGHT_H
#define _DIRLIGHT_H
#include "BaseComponent.h"
#include "Common.h"
#include "Camera.h"

class DirectionalLightComp :
	public BaseComponent
{
private:
	vec4 color = vec4(0.8, 0.8, 0.8, 1);
	vec3 direction = vec3(0, -1, 0);
	vec3 pos = vec3(0, 0, 0);

	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMapFBO;
	GLuint depthMap;
public:
	DirectionalLightComp();
	~DirectionalLightComp();

	vec4 GetColor() { return color; }
	vec3 GetDirection() { return direction; }
	void SetColor(vec4 value) { color = value; }
	void SetDirection(vec3 value) { direction = value; }
	GLuint GetShadowMap() { return depthMap; }

	Camera* ConfigureDirLightCamera();
	void ShadowMapRenderStart();
	void ShadowMapRenderEnd();
};
#endif
