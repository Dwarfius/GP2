#ifndef _SCENE_H
#define _SCENE_H

#include "Common.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "tinyxml2.h"

#include "CameraBehaviour.h"

static enum componentID {
	COMPONENT_NOTFOUND,
	CAMERA_BEHAVIOUR,
	TERRAIN,
	SKYBOX,
	TIMEDAY,
	MOVEGOBEHAVIOUR,
	LIGHT,
	WATERCOMP
};

using namespace tinyxml2;

class Scene
{
private:
	string name;
	Camera *camera;
	ResourceManager* resourceManager;
	map<string, componentID> componentIDValues;

	vector<GameObject*> gameObjects;
	vector<GameObject*> visibleGOs;
	vector<GameObject*> transparentGOs;
	vector<GameObject*> lights;
public:
	Scene(ResourceManager* rM);
	~Scene();

	Camera* GetSceneCamera() { return camera; }

	//adds a new gameobject to the vector
	GameObject* AddGameObject(const string& name, const vec3& position, const vec3& rotation, const vec3& scale, Renderer *r);
	GameObject* AddGameObject(const string& name, const vec3& position, const vec3& rotation, const vec3& scale);
	void AttachComponent(string& compID, GameObject* go, XMLElement* attributesElement);
	//adds a creted gameobject from outside the scene to the scene
	void AddGameObject(GameObject* go);
	//Deletes all the things!
	void ReleaseResources();
	void Update(float deltaTime);
	void VisibilityCheck();
	void Sort(bool (*comparer)(GameObject *a, GameObject *b));
	void Render(Camera* camera);

	int GetLightCount() { return lights.size(); }
	Renderer* GetLight(int i) { return lights[i]->GetRenderer(); }

	int GetGOCount() { return gameObjects.size(); }
	int GetVisibleGOCount() { return visibleGOs.size(); }
};
#endif 
