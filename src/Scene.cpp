#include "Scene.h"
#include "Skybox.h"
#include "TerrainComp.h"
#include "TimeDay.h"
#include "MoveGameObjectBehaviour.h"
#include "Light.h"
#include "WaterComp.h"

Scene::Scene(ResourceManager* rM)
{
	resourceManager = rM;
	//register components here
	componentIDValues[""] = COMPONENT_NOTFOUND;
	componentIDValues["CameraBehaviour"] = CAMERA_BEHAVIOUR;
	componentIDValues["Terrain"] = TERRAIN;
	componentIDValues["Skybox"] = SKYBOX;
	componentIDValues["TimeDay"] = TIMEDAY;
	componentIDValues["MoveGOBehaviour"] = MOVEGOBEHAVIOUR;
	componentIDValues["Light"] = LIGHT;
	componentIDValues["WaterComp"] = WATERCOMP;
	camera = new Camera();
}

Scene::~Scene()
{
}

GameObject* Scene::AddGameObject(const string& name, const vec3& position, const vec3& rotation, const vec3& scale, Renderer *r)
{
	GameObject *go = new GameObject();
	go->SetName(name);
	go->SetPos(position);
	go->SetRotation(rotation);
	go->SetScale(scale);
	go->AttachComponent(r);
	gameObjects.push_back(go);
	return go;
}

GameObject * Scene::AddGameObject(const string & name, const vec3 & position, const vec3 & rotation, const vec3 & scale)
{
	GameObject *go = new GameObject();
	go->SetName(name);
	go->SetPos(position);
	go->SetRotation(rotation);
	go->SetScale(scale);
	gameObjects.push_back(go);
	return go;
}

void Scene::AttachComponent(string & compID, GameObject * go, XMLElement* attributesElement)
{
	string tID = compID;
	auto iter = componentIDValues.find(tID);
	if (iter == componentIDValues.end())
		tID = "";

	switch (componentIDValues[tID]) 
	{
		case COMPONENT_NOTFOUND:
			cout << "Component " << compID << " does not exist" << endl;
			return;
		case CAMERA_BEHAVIOUR:
		{
			float fTemp;
			attributesElement->QueryFloatAttribute("speed", &fTemp);
			go->AttachComponent(new CameraBehaviour(camera, fTemp));
		}
			break;
		case TERRAIN:
		{
			string texture = TEXTURE_PATH + attributesElement->Attribute("texture");
			float scaleX = attributesElement->FloatAttribute("scalex");
			float scaleY = attributesElement->FloatAttribute("scaley");
			float scaleZ = attributesElement->FloatAttribute("scalez");
			go->AttachComponent(new TerrainComp(texture, vec3(scaleX, scaleY, scaleZ)));
		}
			break;
		case SKYBOX:
		{
			string name = attributesElement->Attribute("texture1");
			string name2 = attributesElement->Attribute("texture2");
			bool iTD = false;
			attributesElement->QueryBoolAttribute("isTimeDay", &iTD);
			ResourceManager *rm = resourceManager;
			if (iTD)
				go->AttachComponent(new Skybox(rm->GetTexture(name), rm->GetModel("skyModel"), rm->GetShader("SkyBox"), rm->GetTexture(name2), iTD));
			if (!iTD)
				go->AttachComponent(new Skybox(rm->GetTexture(name), rm->GetModel("skyModel"), rm->GetShader("SkyBox"), rm->GetTexture(name), iTD));
		}
			break;
		case TIMEDAY:
		{
			string tF = attributesElement->Attribute("font");
			go->AttachComponent(new TimeDay(resourceManager->GetFont(tF)));
		}
			break;
		case MOVEGOBEHAVIOUR:
			go->AttachComponent(new MoveGameObjectBehaviour());
			break;
		case LIGHT:
		{
			string colorString = attributesElement->Attribute("color");
			vector<string> splits = split(colorString, ',');
			float r = atof(splits[0].c_str());
			float g = atof(splits[1].c_str());
			float b = atof(splits[2].c_str());
			float a = atof(splits[3].c_str());
			vec4 color(r, g, b, a);
			go->AttachComponent(new Light(color));
		}
			break;
		case WATERCOMP:
		{
			go->AttachComponent(new WaterComp());
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
		(*iter)->Update(deltaTime);
	}
}

void Scene::VisibilityCheck()
{
	//clearing out info from last frame
	visibleGOs.clear();
	lights.clear();
	transparentGOs.clear();

	//repopulating!
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		Renderer *r = (*iter)->GetRenderer();
		if (r)
		{
			Model *m = r->GetModel();
			
			//view frustrum culling
			if (m->UsesBoundSphereTest())
			{
				Sphere sphere = m->GetBoundingSphere((*iter)->GetModelMatrix());

				if (!camera->CheckSphere(sphere.pos, sphere.rad))
					continue;
			}
			
			Light *l = (*iter)->GetLight();
			if (l)
				lights.push_back(*iter);
			else if (r->GetTransparent())
				transparentGOs.push_back(*iter);
			else
				visibleGOs.push_back(*iter);
		}
	}
}

void Scene::Sort(bool (*comparer)(GameObject *a, GameObject *b))
{
	sort(visibleGOs.begin(), visibleGOs.end(), comparer);
}

void Scene::Render(Camera* camera)
{
	for (auto iter = visibleGOs.begin(); iter != visibleGOs.end(); iter++)
		(*iter)->Render(camera);

	//after the scene normal geometry was rendered
	//render out the semitransparent geometry
	if (transparentGOs.size())
	{
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for (auto iter = transparentGOs.begin(); iter != transparentGOs.end(); iter++)
			(*iter)->Render(camera);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
	}
}
