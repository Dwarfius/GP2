#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "Common.h"
#include "BaseComponent.h"
#include "Renderer.h"
#include "Light.h"
#include "Camera.h"
#include <map>

class GameObject
{
private:
	vec3 pos, rotation, size;
	mat4 modelMatrix;
	Renderer *renderer = NULL;
	Light *light = NULL;
	map<string, BaseComponent*> components;
	string name = "default";

	bool modelDirty = false;
	void UpdateMatrix();

public:
	GameObject();
	~GameObject();

	void SetName(string n) { name = n; }
	string GetName() { return name; }

	Renderer* GetRenderer() { return renderer; }
	Light* GetLight() { return light; }

	void Update(float deltaTime);
	void Render(Camera *camera, ShaderProgram *OverrideProgram = nullptr);

	vec3 GetPos() { return pos; }
	void SetPos(vec3 pPos) { pos = pPos; modelDirty = true; }
	void Move(vec3 delta) { pos += delta; modelDirty = true; }

	vec3 GetRotation() { return rotation; }
	void SetRotation(vec3 pRotation) { rotation = pRotation; modelDirty = true; }
	void AddRotation(vec3 delta) { rotation += delta; modelDirty = true; }

	vec3 GetScale() { return size; }
	void SetScale(vec3 pScale) { size = pScale; modelDirty = true; }
	void AddScale(vec3 delta) { size += delta; modelDirty = true; }

	mat4 GetModelMatrix() { if (modelDirty) UpdateMatrix(); return modelMatrix; }

	void AttachComponent(BaseComponent *com);
	BaseComponent* GetComponent(string componentType);
};

#endif