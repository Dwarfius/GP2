#include "SceneManager.h"
#include "GameObject.h"

SceneManager::SceneManager()
{
	sceneMgrConfigPath = GAMEDATA_PATH + "sceneMgrConfig.scn";
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

void SceneManager::LoadSceneDirectories()
{
	XMLDocument* gamedataXML = new XMLDocument();
	loadXML(sceneMgrConfigPath, gamedataXML);
	XMLNode * SMCroot = gamedataXML->FirstChild();
	if (SMCroot == nullptr) {
		XML_ERROR_FILE_READ_ERROR;
		return;
	}
	XMLElement * SMCdirectories = SMCroot->FirstChildElement("SceneDirectories");
	if (SMCdirectories == nullptr){
		XML_ERROR_PARSING_ELEMENT; 
		return;
	}
	XMLElement * DirectoryItem = SMCdirectories->FirstChildElement("Scene");
	if (SMCdirectories == nullptr) {
		XML_ERROR_PARSING_ELEMENT;
		return;
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
}

void SceneManager::LoadScene(int sceneOrder, Scene* currentScene)
{
	XMLDocument* gamedataXML = new XMLDocument();
	loadXML(GAMEDATA_PATH + scenes[sceneOrder], gamedataXML);
	XMLNode * sceneRoot = gamedataXML->FirstChild();
	if (sceneRoot == nullptr) {
		XML_ERROR_FILE_READ_ERROR;
		return;
	}

	XMLElement * sceneData = sceneRoot->FirstChildElement("ModelList");
	if (sceneData == nullptr) {
		XML_ERROR_PARSING_ELEMENT;
		return;
	}
	XMLElement * sceneItem = sceneData->FirstChildElement("Model");
	if (sceneData == nullptr) {
		XML_ERROR_PARSING_ELEMENT;
		return;
	}
	while (sceneItem != nullptr) {
		currentScene->AddModel(sceneItem->GetText());
		sceneItem = sceneItem->NextSiblingElement("Model");
	}

	sceneData = sceneData->NextSiblingElement("TextureList");
	if (sceneData == nullptr) {
		XML_ERROR_PARSING_ELEMENT;
		return;
	}
	sceneItem = sceneData->FirstChildElement("Texture");
	if (sceneData == nullptr) {
		XML_ERROR_PARSING_ELEMENT;
		return;
	}
	while (sceneItem != nullptr) {
		currentScene->AddTexture(sceneItem->GetText());
		sceneItem = sceneItem->NextSiblingElement("Texture");
	}

	sceneData = sceneData->NextSiblingElement("GameObjectList");
	if (sceneData == nullptr) {
		XML_ERROR_PARSING_ELEMENT;
		return;
	}
	sceneItem = sceneData->FirstChildElement("GameObject");
	if (sceneData == nullptr) {
		XML_ERROR_PARSING_ELEMENT;
		return;
	}
	while (sceneItem != nullptr) {
		string tName ="";
		string tTextureName;
		string tModelName;
		string tShaderProgramName;
		tName = sceneItem->Attribute("name");
		tTextureName = sceneItem->Attribute("texture");
		currentScene->AddTexture(tTextureName);
		tModelName = sceneItem->Attribute("model");
		currentScene->AddModel(tModelName);
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
		currentScene->NewGameObject(tName, tTextureName, tModelName, tShaderProgramName, tPos, tRot, tScale);
		sceneItem = sceneItem->NextSiblingElement("GameObject");
	}
	delete gamedataXML;
}

void SceneManager::LoadScene(string lvlName, Scene* currentScene)
{
	int sceneOrder = NULL;
	for (auto iter = scenes.begin(); iter != scenes.end(); iter++)
	{
		if (iter->second == lvlName + ".scn") {
			sceneOrder = iter->first;
		}
	}
	if (sceneOrder != NULL) {
		return;
	}
	else {
		LoadScene(sceneOrder, currentScene);
	}
}

void SceneManager::ReleaseResources()
{
}
