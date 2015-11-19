#include "Scene.h"

Scene::Scene(ResourceManager* rM)
{
	resourceManager = rM;
}

Scene::~Scene()
{
}

void Scene::NewGameObject(string& n, string& t, string& m, string& s, vec3& position, vec3& rotation, vec3& scale)
{
	GameObject *go = new GameObject();
	go->SetName(n);
	go->SetPos(position);
	go->SetRotation(rotation);
	go->SetScale(scale);
	Renderer *r = new Renderer();
	r->SetModel(resourceManager->GetModel(m), GL_TRIANGLES);
	r->SetTexture(resourceManager->GetTexture(t));
	r->SetShaderProgram(resourceManager->GetShader(s));
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

