#ifndef _LEVELMANAGER_H
#define _LEVELMANAGER_H

#include "Common.h"
#include "Scene.h"
#include "tinyxml2.h"
#include <map>

using namespace tinyxml2;

class SceneManager 
{
private:
	Scene * currentScene;
	XMLError eResult;
	XMLDocument* sceneMgrConfigXML = new XMLDocument();
	string sceneMgrConfigPath;
	
public:
	map<int, string> scenes;
	SceneManager();
	~SceneManager();
	void loadXML(string filename, XMLDocument* xml);
	void LoadSceneDirectories();
	void LoadScene(int lvlOrder);
	void LoadScene(string lvlName);
	void Update(float deltaTime);

};
#endif 