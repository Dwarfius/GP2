#ifndef _TERRAIN_COMP_H
#define _TERRAIN_COMP_H

#include "Common.h"
#include "BaseComponent.h"
#include "Vertex.h"

class TerrainComp : public BaseComponent
{
private:
	vector<Vertex> *vertices;
	vector<int> *indices;

	//returns normalized vector of RGB color
	vec3 GetPixel(SDL_Surface *s, int x, int y);

public:
	TerrainComp(const string &fileName, vec3 size);
	~TerrainComp();

	void SetParentGO(GameObject *pGO);
};

#endif