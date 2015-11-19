#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "common.h"
#include "GameObject.h"
#include <map>

class ResourceManager {
private:
map<string, ShaderProgram*> shaderPrograms;
map<string, Texture*> textures;
map<string, Model*> models;

Texture* currentTexture;
Shader* currentShader;
Model* currentModel;

public:
	ResourceManager();
	~ResourceManager();
//adds shader to the map, should be done at the start of application
void AddShader(ShaderProgram* shader, string name);
//adds model to the model map
void AddModel(const string& filename);
//adds texture to the map
void AddTexture(const string& filename);
ShaderProgram* GetShader(string& name);
Texture* GetTexture(string& name);
Model* GetModel(string& name);
//Deletes all the things!
void ReleaseResources();
};
#endif