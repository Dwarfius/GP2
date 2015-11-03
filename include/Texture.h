#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "Common.h"

class Texture
{
private:
	GLuint textureId;

	GLuint LoadTextureFromFile(const string& fileName);
	GLuint ConvertSDLSurfaceToTexture(SDL_Surface *surf);

public:
	Texture(GLuint text) { textureId = text; }
	Texture(const string& fileName);
	~Texture();

	GLuint Get() { return textureId; }
};

#endif