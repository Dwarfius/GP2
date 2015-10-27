#include "GameObject.h"

GameObject::GameObject()
{
	pos = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);
	size = vec3(1, 1, 1);
}

GameObject::~GameObject()
{
	if (!renderer)
		delete renderer;
}

void GameObject::Update(float deltaTime)
{
	for (vector<BaseComponent*>::iterator it = components.begin(); it != components.end(); it++) {
		(*it)->Update(deltaTime);
	}
}

void GameObject::Render(Camera *camera)
{
	modelMatrix = scale(mat4(1), size);
	modelMatrix = rotate(modelMatrix, rotation.x, vec3(1, 0, 0));
	modelMatrix = rotate(modelMatrix, rotation.y, vec3(0, 1, 0));
	modelMatrix = rotate(modelMatrix, rotation.z, vec3(0, 0, 1));
	modelMatrix = translate(modelMatrix, pos);

	renderer->Render(modelMatrix, camera->Get());
}

void GameObject::AttacheComponent(BaseComponent * com)
{
	components.push_back(com); 
	com->SetParentGO(com->GetParentGO());
}
