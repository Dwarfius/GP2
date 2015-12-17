#include "SceneManager.h"
#include "ShaderProgram.h"
#include "GameObject.h"

SceneManager::SceneManager(ResourceManager* rM)
{
	sceneMgrConfigPath = GAMEDATA_PATH + "sceneMgrConfig.scn";
	resourceManager = rM;
}

SceneManager::~SceneManager()
{
}

void xmlErrorCheck(XMLError error, string msg) {
	if (error == XML_SUCCESS) {
		cout << "XML: " << msg << " success!" << endl;
	}
	else {
		cout << "ERROR XML: " << msg << " failed!" << endl;
	}
}

void SceneManager::loadXML(string filename, XMLDocument* xml)
{
	eResult = xml->LoadFile(filename.c_str());
	xmlErrorCheck(eResult, "File loading");
}

XMLError SceneManager::LoadSceneDirectories()
{
	XMLDocument* gamedataXML = new XMLDocument();
	loadXML(sceneMgrConfigPath, gamedataXML);
	XMLNode * SMCroot = gamedataXML->FirstChild();
	if (SMCroot == nullptr) {
		return XML_ERROR_FILE_READ_ERROR;
	}
	XMLElement * SMCdirectories = SMCroot->FirstChildElement("SceneDirectories");
	if (SMCdirectories == nullptr){
		return XML_ERROR_PARSING_ELEMENT;
	}
	XMLElement * DirectoryItem = SMCdirectories->FirstChildElement("Scene");
	if (SMCdirectories == nullptr) {
		return XML_ERROR_PARSING_ELEMENT;
	}
	while (DirectoryItem != nullptr) {
		int tOrder;
		string tDirectory;
		eResult = DirectoryItem->QueryIntAttribute("order", &tOrder);
		xmlErrorCheck(eResult, "SceneDirectoryItem loading");
		tDirectory = DirectoryItem->GetText();
		scenes[tOrder] = tDirectory;
		DirectoryItem = DirectoryItem->NextSiblingElement("Scene");
	}
	delete gamedataXML;
	return XML_SUCCESS;
}

XMLError SceneManager::LoadScene(int sceneOrder, Scene* currentScene)
{
	XMLDocument* gamedataXML = new XMLDocument();
	loadXML(GAMEDATA_PATH + scenes[sceneOrder], gamedataXML);
	XMLNode * sceneRoot = gamedataXML->FirstChild();
	if (sceneRoot == nullptr)
		return XML_ERROR_FILE_READ_ERROR;

	XMLElement *sceneData = sceneRoot->FirstChildElement("ModelList");
	if (sceneData == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	XMLElement *sceneItem = sceneData->FirstChildElement("Model");
	while (sceneItem) 
	{
		resourceManager->AddModel(sceneItem->GetText());
		sceneItem = sceneItem->NextSiblingElement("Model");
	}

	sceneData = sceneRoot->FirstChildElement("TextureList");
	if (sceneData == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	sceneItem = sceneData->FirstChildElement("Texture");
	while (sceneItem) 
	{
		resourceManager->AddTexture(sceneItem->GetText());
		sceneItem = sceneItem->NextSiblingElement("Texture");
	}

	sceneData = sceneRoot->FirstChildElement("FontList");
	if (sceneData == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	sceneItem = sceneData->FirstChildElement("Font");
	while (sceneItem) 
	{
		resourceManager->AddFont(sceneItem->GetText());
		sceneItem = sceneItem->NextSiblingElement("Font");
	}

	sceneData = sceneRoot->FirstChildElement("ShaderList");
	if (!sceneData)
		return XML_ERROR_PARSING_ELEMENT;
	sceneItem = sceneData->FirstChildElement("Shader");
	while (sceneItem)
	{
		string name = sceneItem->GetText();
		string vert = sceneItem->Attribute("vert");
		string frag = sceneItem->Attribute("frag");
		ShaderProgram *shader = new ShaderProgram(SHADER_PATH + vert, SHADER_PATH + frag);
		shader->Link();
		resourceManager->AddShader(shader, name);
		sceneItem = sceneItem->NextSiblingElement("Shader");
	}

	sceneData = sceneRoot->FirstChildElement("GameObjectList");
	if (sceneData == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	sceneItem = sceneData->FirstChildElement("GameObject");
	while (sceneItem) 
	{
		string tName ="";
		string tTextureName;
		string tModelName;
		string tShaderProgramName;
		bool hasRenderer = true;
		tName = sceneItem->Attribute("name");
		sceneItem->QueryBoolAttribute("hasRenderer", &hasRenderer);
		string textures = sceneItem->Attribute("texture");
		vector<string> splits = split(textures, ',');

		tModelName = sceneItem->Attribute("model");
		tShaderProgramName = sceneItem->Attribute("shader");

		vec3 tPos;
		sceneItem->QueryFloatAttribute("posx", &tPos.x);
		sceneItem->QueryFloatAttribute("posy", &tPos.y);
		sceneItem->QueryFloatAttribute("posz", &tPos.z);
		vec3 tRot;
		sceneItem->QueryFloatAttribute("rotx", &tRot.x);
		sceneItem->QueryFloatAttribute("roty", &tRot.y);
		sceneItem->QueryFloatAttribute("rotz", &tRot.z);
		vec3 tScale;
		sceneItem->QueryFloatAttribute("scalex", &tScale.x);
		sceneItem->QueryFloatAttribute("scaley", &tScale.y);
		sceneItem->QueryFloatAttribute("scalez", &tScale.z);

		GameObject *go;
		if (hasRenderer) 
		{
			Renderer *renderer = new Renderer();
			renderer->SetTransparent(sceneItem->Attribute("transparent", "true"));
			if (tModelName.length() > 0)
			{
				Model *m = resourceManager->GetModel(tModelName);
				if (m)
					renderer->SetModel(m, GL_TRIANGLES);
			}

			for (int i = 0; i < splits.size(); i++)
				renderer->AddTexture(resourceManager->GetTexture(splits[i]));

			if (tShaderProgramName.length() > 0)
			{
				ShaderProgram *shader = resourceManager->GetShader(tShaderProgramName);
				if (shader)
					renderer->SetShaderProgram(shader);
			}
			go = currentScene->AddGameObject(tName, tPos, tRot, tScale, renderer);
		}
		else { go = currentScene->AddGameObject(tName, tPos, tRot, tScale); }

		XMLElement * gameObjectElement = sceneItem->FirstChildElement("ComponentList");
		XMLElement * componentElement = gameObjectElement->FirstChildElement("Component");
		while (componentElement != nullptr) {
			string compID = componentElement->Attribute("ID");
			XMLElement * attributesElement = componentElement->FirstChildElement("Attributes");
			currentScene->AttachComponent(compID, go, attributesElement);
			componentElement = componentElement->NextSiblingElement("Component");
		}
		sceneItem = sceneItem->NextSiblingElement("GameObject");
	}
	currentScene->SetMainDirLight();
	delete gamedataXML;
	return XML_SUCCESS;
}

XMLError SceneManager::LoadScene(string lvlName, Scene* currentScene)
{
	int sceneOrder = NULL;
	for (auto iter = scenes.begin(); iter != scenes.end(); iter++)
	{
		if (iter->second == lvlName + ".scn") {
			sceneOrder = iter->first;
		}
	}
	if (sceneOrder != NULL) {
		return XML_ERROR_FILE_NOT_FOUND;
	}
	else {
		return LoadScene(sceneOrder, currentScene);
	}
}
