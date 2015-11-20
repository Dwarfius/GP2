#include "BaseComponent.h"

BaseComponent::BaseComponent()
{
	pGameObject = 0;
}

BaseComponent::~BaseComponent()
{
}

void BaseComponent::SetParentGO(GameObject *pGO)
{
	pGameObject = pGO;
}

inline GameObject* BaseComponent::GetParentGO()
{
	return pGameObject;
}

void BaseComponent::Update(float deltaTime)
{
}
