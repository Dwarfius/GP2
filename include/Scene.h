#ifndef _SCENE_H
#define _SCENE_H

#include "Common.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include <map>

class Scene
{
private:
	string name;
	Camera *camera;
	vector<GameObject*> gameObjects;
	ResourceManager* resourceManager;
public:
	Scene(ResourceManager* rM);
	~Scene();
	//adds a new gameobject to the vector
	void NewGameObject(string& n, string& t, string& m, 
		string& s, vec3& position, vec3& rotation, vec3& scale);
	//adds a creted gameobject from outside the scene to the scene
	void AddGamObject(GameObject* go);
	//Deletes all the things!
	void ReleaseResources();
	void Update(float deltaTime);
	void Render(Camera* camera);
};
#endif 
