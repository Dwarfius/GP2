#ifndef _SCENE_H
#define _SCENE_H

#include "Common.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "tinyxml2.h"
#include <map>

#include "CameraBehaviour.h"
#include "TerrainComp.h"

static enum componentID {
	COMPONENT_NOTFOUND,
	CAMERA_BEHAVIOUR,
	TERRAIN,
	SKYBOX
};

using namespace tinyxml2;

class Scene
{
private:
	string name;
	Camera *camera;
	ResourceManager* resourceManager;
	map<string, componentID> componentIDValues;
public:
	vector<GameObject*> gameObjects;
	Scene(ResourceManager* rM);
	~Scene();

	Camera* GetSceneCamera() { return camera; }

	//adds a new gameobject to the vector
	void AddGameObject(string& n, string& t, string& m, 
		string& s, vec3& position, vec3& rotation, vec3& scale, GameObject* go);
	void AttachComponent(string& compID, GameObject* go, XMLElement* attributesElement);
	//adds a creted gameobject from outside the scene to the scene
	void AddGameObject(GameObject* go);
	//Deletes all the things!
	void ReleaseResources();
	void Update(float deltaTime);
	void Render(Camera* camera);
};
#endif 
