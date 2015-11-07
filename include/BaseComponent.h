#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H

#include "Common.h"

class GameObject;

class BaseComponent
{
protected:
	GameObject *pGameObject;
public:
	BaseComponent();
	~BaseComponent();

	void SetParentGO(GameObject *pgo) { pGameObject = pgo; }
	GameObject* GetParentGO() { return pGameObject; }

	virtual void Update(float deltaTime);
};
#endif