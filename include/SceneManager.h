#ifndef _LEVELMANAGER_H
#define _LEVELMANAGER_H

#include "Common.h"
#include "Scene.h"
#include "tinyxml2.h"
#include <map>

class SceneManager 
{
private:
	Scene currentScene;
	string sceneMgrConfig = GAMEDATA_PATH + "levelMgrConfig.xml";
	map<pair<int, string>, string> levels;
public:
	SceneManager();
	~SceneManager();
	void LoadScene(int lvlOrder);
	void LoadScene(string lvlName);
	void Update(float deltaTime);

};
#endif 