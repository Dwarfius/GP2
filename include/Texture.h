#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "Common.h"

class Texture
{
private:
	GLuint textureId;

	GLuint LoadTextureFromFile(const string& fileName);
	GLuint LoadTextureFromFont(const string& fileName, int size, const string& text);
	GLuint ConvertSDLSurfaceToTexture(SDL_Surface *surf);

public:
	Texture(const string& fileName);
	Texture(const string& font, const string& text);
	~Texture();

	GLuint Get() { return textureId; }
};

#endif