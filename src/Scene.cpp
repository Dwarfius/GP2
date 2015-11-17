#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::AddShader(ShaderProgram * shader, string name)
{
	shaderPrograms[name] = shader;
}

void Scene::AddModel(const string& filename)
{
	models[filename] = new Model(MODEL_PATH + filename);
}

void Scene::AddTexture(const string & filename)
{
	textures[filename] = new Texture(TEXTURE_PATH + filename);
}

void Scene::NewGameObject(string& n, string& t, string& m, string& s, vec3& position, vec3& rotation, vec3& scale)
{
	GameObject *go = new GameObject();
	go->SetName(n);
	go->SetPos(position);
	go->SetRotation(rotation);
	go->SetScale(scale);
	Renderer *r = new Renderer();
	r->SetModel(models[m], GL_TRIANGLES);
	r->SetTexture(textures[t]);
	r->SetShaderProgram(shaderPrograms[s]);
	go->SetRenderer(r);
	gameObjects.push_back(go);
}

void Scene::AddGamObject(GameObject * go)
{
	gameObjects.push_back(go);
}

void Scene::ReleaseResources()
{
	//clearing out all the objects
	int count = gameObjects.size();
	while (count-- > 0)
	{
		delete gameObjects[count];
		gameObjects.pop_back();
	}

	for(auto iter = textures.begin(); iter != textures.end(); iter++)
	{
		delete iter->second;
	}

	for (auto iter = models.begin(); iter != models.end(); iter++)
	{
		delete iter->second;
	}
}

void Scene::Update(float deltaTime)
{
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		(*iter)->AddRotation(vec3(0, deltaTime * 15, 0));
		(*iter)->Update(deltaTime);
	}
}

void Scene::Render(Camera* camera)
{
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
		(*iter)->Render(camera);
}

