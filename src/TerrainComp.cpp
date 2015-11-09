#include "TerrainComp.h"
#include "GameObject.h"

TerrainComp::TerrainComp(const string &fileName, vec3 size)
{
	//parsing the heightmap
	vertices = new vector<Vertex>();
	indices = new vector<int>();

	SDL_Surface *surf = IMG_Load(fileName.c_str());
	if (!surf)
		printf("Couldn't load image %s-%s\n", fileName.c_str(), IMG_GetError());
	int w = surf->w;
	float wStep = size.x / w;
	int h = surf->h;
	float hStep = size.y / h;
	
	//first, creating the vertices
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			vec3 pixel = GetPixel(surf, x, y);
			Vertex v;
			v.pos.x = x * wStep;
			v.pos.z = y * hStep;
			v.pos.y = size.z * pixel.r;
			v.texture.x = x % 2;
			v.texture.y = y % 2;
			vertices->push_back(v);
		}
	}

	//now creating indices
	for (int y = 0; y < h - 1; y++)
	{
		for (int x = 0; x < w - 1; x++)
		{
			int tl = y * w + x;
			int tr = tl + 1;
			int bl = tl + w;
			int br = bl + 1;
			indices->push_back(tl);
			indices->push_back(bl);
			indices->push_back(tr);
			indices->push_back(br);
			indices->push_back(tr);
			indices->push_back(bl);
		}
	}
	SDL_FreeSurface(surf);
}

TerrainComp::~TerrainComp()
{
}

void TerrainComp::SetParentGO(GameObject *pGO)
{
	pGameObject = pGO;

	//attaching the mesh generated based on a heightmap
	Renderer *renderer = (Renderer*)pGameObject->GetComponent("Renderer");
	Model *model = renderer->GetModel();
	model->SetVertices(vertices, GL_STATIC_DRAW, true);
	model->SetIndices(indices, GL_STATIC_DRAW, true);
}

//http://stackoverflow.com/questions/17270538/how-to-change-rgb-values-in-sdl-surface
vec3 TerrainComp::GetPixel(SDL_Surface *s, int x, int y)
{
	uint32 pixel = *(uint32*)((uint8 *)s->pixels + y * s->pitch + x * s->format->BytesPerPixel);

	uint8 r, g, b;
	SDL_GetRGB(pixel, s->format, &r, &g, &b);
	return vec3(r / 255.f, g / 255.f, b / 255.f);
}