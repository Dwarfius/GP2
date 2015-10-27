#pragma once
#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H
#include "Common.h"

class GameObject;

class BaseComponent
{
protected:
public:
	GameObject gameObject;

	BaseComponent();
	~BaseComponent();

	void SetParentGO(GameObject *pgo) { gameObject = pgo; }
	GameObject* GetParentGO() { return gameObject; }

	void Update(float deltaTime);
};
#endif