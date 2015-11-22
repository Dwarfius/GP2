#include "SceneManager.h"
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
	if (sceneRoot == nullptr) {
		return XML_ERROR_FILE_READ_ERROR;
	}

	XMLElement * sceneData = sceneRoot->FirstChildElement("ModelList");
	if (sceneData == nullptr) {
		return XML_ERROR_PARSING_ELEMENT;
	}
	XMLElement * sceneItem = sceneData->FirstChildElement("Model");
	if (sceneData == nullptr) {
		return XML_ERROR_PARSING_ELEMENT;
	}
	while (sceneItem != nullptr) {
		resourceManager->AddModel(sceneItem->GetText());
		sceneItem = sceneItem->NextSiblingElement("Model");
	}

	sceneData = sceneData->NextSiblingElement("TextureList");
	if (sceneData == nullptr) {
		return XML_ERROR_PARSING_ELEMENT;
	}
	sceneItem = sceneData->FirstChildElement("Texture");
	if (sceneData == nullptr) {
		return XML_ERROR_PARSING_ELEMENT;
	}
	while (sceneItem != nullptr) {
		resourceManager->AddTexture(sceneItem->GetText());
		sceneItem = sceneItem->NextSiblingElement("Texture");
	}

	sceneData = sceneData->NextSiblingElement("GameObjectList");
	if (sceneData == nullptr) {
		return XML_ERROR_PARSING_ELEMENT;
	}
	sceneItem = sceneData->FirstChildElement("GameObject");
	if (sceneData == nullptr) {
		return XML_ERROR_PARSING_ELEMENT;
	}
	while (sceneItem != nullptr) {
		string tName ="";
		string tTextureName;
		string tModelName;
		string tShaderProgramName;
		tName = sceneItem->Attribute("name");
		tTextureName = sceneItem->Attribute("texture");
		resourceManager->AddTexture(tTextureName);
		tModelName = sceneItem->Attribute("model");
		resourceManager->AddModel(tModelName);
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

		GameObject * go = new GameObject();

		bool hasModel = true;
		sceneItem->QueryBoolAttribute("hasModel", &hasModel);
		if (hasModel) {
			currentScene->AddGameObject(tName, tTextureName, tModelName, tShaderProgramName, tPos, tRot, tScale, go);
		}
		else { currentScene->AddGameObject(go); }

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
