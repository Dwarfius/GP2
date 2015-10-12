#include "GameObject.h"

GameObject::GameObject()
{
	pos = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);
	size = vec3(1, 1, 1);
	renderer = new Renderer();
}

GameObject::~GameObject()
{
	delete renderer;
}

void GameObject::Update(float deltaTime)
{

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