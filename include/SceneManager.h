#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include "Common.h"
#include "Scene.h"
#include "tinyxml2.h"
#include <map>

using namespace tinyxml2;

class SceneManager 
{
private:
	XMLError eResult;
	string sceneMgrConfigPath;
	ResourceManager* resourceManager;
	
public:
	map<int, string> scenes;
	SceneManager();
	~SceneManager();
	//loads and xml file
	void loadXML(string filename, XMLDocument* xml);
	//loads the scene directories for all the scenes in the deliverable
	void LoadSceneDirectories();
	//loads the scene, takes in the scene order int as a parameter
	void LoadScene(int lvlOrder, Scene* currentScene);
	//loads the scene, takes in the scene name(without the ".scn") as a parameter
	void LoadScene(string lvlName, Scene* currentScene);
};
#endif 