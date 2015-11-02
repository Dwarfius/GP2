#ifndef _FONT_H
#define _FONT_H

#include "Common.h"
#include "Vertex.h"
#include "Model.h"
#include "Renderer.h"

#define FONT_TEXTURE_SLOT 20;

class Font
{
private:
	//the size of the atlas texture
	int width = 512, height = 512;
	//contains normalized UVs
	SDL_Rect rects[127 - 32];
	//contains vertices for submitting to gpu
	vector<Vertex> *vertices;
	vector<int> *indices;
	Renderer *renderer;
	Model *m;
	Texture *t;
	ShaderProgram *shader;

	GLuint Font::ConvertSDLSurfaceToTexture(SDL_Surface *surf);

public:
	Font(const string& fileName);
	~Font();

	void Render(const string& text, const SDL_Rect rect);
	void Flush(Camera *cam);
};

#endif