#include "SceneManager.h"

SceneManager::SceneManager()
{
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

void SceneManager::loadXML(const char* filename, XMLDocument* xml)
{
	eResult = xml->LoadFile(filename);
	xmlErrorCheck(eResult, "File loading");
}

void SceneManager::LoadSceneDirectories()
{
	const char * tchar = sceneMgrConfigPath.c_str();
	loadXML(tchar, sceneMgrConfigXML);
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
		eResult = DirectoryItem->QueryIntAttribute("Order", &tOrder);
		xmlErrorCheck(eResult, "SceneDirectoryItem loading");
		tDirectory = DirectoryItem->GetText();
		scenes[tOrder] = tDirectory;
		DirectoryItem = SMCdirectories->NextSiblingElement("Scene");
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
