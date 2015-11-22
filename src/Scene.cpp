#include "Scene.h"
#include "Skybox.h"

Scene::Scene(ResourceManager* rM)
{
	resourceManager = rM;
	//register components here
	componentIDValues[""] = COMPONENT_NOTFOUND;
	componentIDValues["CameraBehaviour"] = CAMERA_BEHAVIOUR;
	componentIDValues["Terrain"] = TERRAIN;
	componentIDValues["Skybox"] = SKYBOX;
	camera = new Camera();
}

Scene::~Scene()
{
}

void Scene::AddGameObject(string& n, string& t, string& m, string& s, vec3& position, vec3& rotation, vec3& scale, GameObject* go)
{
	go->SetName(n);
	go->SetPos(position);
	go->SetRotation(rotation);
	go->SetScale(scale);
	Renderer *r = new Renderer();
	r->SetModel(resourceManager->GetModel(m), GL_TRIANGLES);
	r->AddTexture(resourceManager->GetTexture(t));
	r->SetShaderProgram(resourceManager->GetShader(s));
	go->AttachComponent(r);
	gameObjects.push_back(go);
}

void Scene::AttachComponent(string & compID, GameObject * go, XMLElement* attributesElement)
{
	string tID = compID;
	auto iter = componentIDValues.find(tID);
	if (iter == componentIDValues.end()) {
		tID = "";
	}
	switch (componentIDValues[tID]) {
	case COMPONENT_NOTFOUND:
		cout << "Component " << compID << " does not exist" << endl;
		return;
		break;
	case CAMERA_BEHAVIOUR: {
		float fTemp;
		attributesElement->QueryFloatAttribute("speed", &fTemp);
		go->AttachComponent(new CameraBehaviour(camera, fTemp));
	}
		break;
	case TERRAIN:
		//go->AttachComponent(new TerrainComp());
		break;
	case SKYBOX:
	{
		string name = attributesElement->Attribute("texture");
		ResourceManager *rm = resourceManager;
		go->AttachComponent(new Skybox(rm->GetTexture(name), rm->GetModel("skyModel"), rm->GetShader("SkyBox")));
	}
		break;
	}
}

void Scene::AddGameObject(GameObject * go)
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

	delete camera;
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

