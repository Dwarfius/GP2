#ifndef _LEVEL_H
#define _LEVEL_H

#include "Common.h"
#include "tinyxml2.h"

class Scene
{
private:
	string name;
public:
	Scene();
	~Scene();
	void Update();
};
#endif 
