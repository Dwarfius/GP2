#ifndef _SHADOWCOMP_H
#define _SHADOWCOMP_H
#include "BaseComponent.h"
class ShadowComp :
	public BaseComponent
{
public:
	ShadowComp();
	~ShadowComp();

	void Update(float deltaTime);
	void OnRender(Camera *camera);
};
#endif

