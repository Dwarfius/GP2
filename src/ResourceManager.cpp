#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::AddShader(ShaderProgram * shader, string name)
{
	shaderPrograms[name] = shader;
}

void ResourceManager::AddModel(const string & filename)
{
	if(models.find(filename) == models.end())
		models[filename] = new Model(MODEL_PATH + filename);
}

void ResourceManager::AddTexture(const string & filename)
{
	textures[filename] = new Texture(TEXTURE_PATH + filename);
}

ShaderProgram* ResourceManager::GetShader(string name)
{
	return shaderPrograms[name];
}

Texture* ResourceManager::GetTexture(string & name)
{
	return textures[name];
}

Model* ResourceManager::GetModel(string & name)
{
	return models[name];
}

void ResourceManager::ReleaseResources()
{
	for (auto iter = textures.begin(); iter != textures.end(); iter++)
	{
		delete iter->second;
	}

	for (auto iter = models.begin(); iter != models.end(); iter++)
	{
		delete iter->second;
	}
}
