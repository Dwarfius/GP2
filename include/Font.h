#ifndef _FONT_H
#define _FONT_H

#include "Common.h"

#define FONT_TEXTURE_SLOT 20;

class Font
{
private:
	GLuint atlasText;
	//the size of the atlas texture
	int width = 512, height = 512;
	//contains non-normalized UVs
	SDL_Rect rects[127 - 32];

	GLuint Font::ConvertSDLSurfaceToTexture(SDL_Surface *surf);

public:
	Font(const string& fileName);
	~Font();

	void Render(const string& text, const SDL_Rect rect);
};

#endif