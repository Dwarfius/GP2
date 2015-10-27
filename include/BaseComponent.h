#pragma once
#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H
#include "Common.h"

class GameObject;

class BaseComponent
{
protected:
public:
	GameObject *parentGO;

	BaseComponent();
	~BaseComponent();

	void SetParentGO(GameObject *pgo) { parentGO = pgo; }
	GameObject* GetParentGO() { return parentGO; }

	void Update(float deltaTime);
};
#endif