#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H

#include "Common.h"

class GameObject;

class BaseComponent
{
protected:
	GameObject *gameObject;
public:
	BaseComponent();
	~BaseComponent();

	void SetParentGO(GameObject *pgo) { gameObject = pgo; }
	GameObject* GetParentGO() { return gameObject; }

	virtual void Update(float deltaTime);
};
#endif