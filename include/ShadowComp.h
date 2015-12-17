#ifndef _SHADOWCOMP_H
#define _SHADOWCOMP_H
#include "BaseComponent.h"
#include "Common.h"
class ShadowComp :
	public BaseComponent
{
private:
	mat4 LSM;
	GLuint shadowMap;
public:
	ShadowComp();
	~ShadowComp();

	void Update(float deltaTime);
	void OnRender(Camera *camera);
	void SetShadowUniforms(mat4 tLSM, GLuint tshadowMap);
};
#endif

