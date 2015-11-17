#ifndef _LEVEL_H
#define _LEVEL_H

#include "Common.h"
#include "GameObject.h"
#include <map>

class Scene
{
private:
	string name;
	Camera *camera;
	vector<GameObject*> gameObjects;
	map<string, ShaderProgram*> shaderPrograms;
	map<string, Texture*> textures;
	map<string, Model*> models;
public:
	Scene();
	~Scene();
	//adds shader to the map, should be done at the start of application
	void AddShader(ShaderProgram* shader, string name);
	//adds model to the model map
	void AddModel(const string& filename);
	//adds texture to the map
	void AddTexture(const string& filename);
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
