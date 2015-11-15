#include "SceneManager.h"

SceneManager::SceneManager()
{
	sceneMgrConfigPath = GAMEDATA_PATH + "sceneMgrConfig.xml";
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
	loadXML(sceneMgrConfigPath, sceneMgrConfigXML);
	XMLNode * SMCroot = sceneMgrConfigXML->FirstChild();
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
}

void SceneManager::LoadScene(int lvlOrder)
{

}

void SceneManager::LoadScene(string lvlName)
{

}

void SceneManager::Update(float deltaTime)
{
	currentScene->Update(deltaTime);
}
