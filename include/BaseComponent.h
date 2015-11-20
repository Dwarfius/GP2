#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H

//forward declaring to awoid circular includes
class GameObject;

class BaseComponent
{
protected:
	GameObject *pGameObject;

public:
	BaseComponent();
	~BaseComponent();

	virtual void SetParentGO(GameObject *pGO);
	GameObject* GetParentGO();

	virtual void Update(float deltaTime);
};
#endif