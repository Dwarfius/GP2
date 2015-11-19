#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "Common.h"
#include "BaseComponent.h"
#include "Renderer.h"
#include "Camera.h"
#include <map>

class GameObject
{
private:
	vec3 pos, rotation, size;
	mat4 modelMatrix;
	Renderer *renderer = NULL;
	map<string, BaseComponent*> components;
	string name = "default";

public:
	GameObject();
	~GameObject();

	void SetName(string n) { name = n; }
	string GetName() { return name; }

	void SetRenderer(Renderer *r) { renderer = r; }
	Renderer* GetRenderer() { return renderer; }

	void Update(float deltaTime);
	void Render(Camera *camera);

	vec3 GetPos() { return pos; }
	void SetPos(vec3 pPos) { pos = pPos; }
	void Move(vec3 delta) { pos += delta; }

	vec3 GetRotation() { return rotation; }
	void SetRotation(vec3 pRotation) { rotation = pRotation; }
	void AddRotation(vec3 delta) { rotation += delta; }

	vec3 GetScale() { return size; }
	void SetScale(vec3 pScale) { size = pScale; }
	void AddScale(vec3 delta) { size += delta; }

	mat4 GetModelMatrix() { return modelMatrix; }

	void AttachComponent(BaseComponent *com);
	BaseComponent* GetComponent(string componentType);
};

#endif