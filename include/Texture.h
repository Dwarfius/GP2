#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "Common.h"

class Texture
{
private:
	GLuint textureId;

	GLuint LoadTextureFromFile(const string& fileName);
	GLuint LoadCubmapFromFiles(
		const string& posX, 
		const string& negX, 
		const string& posY, 
		const string& negY, 
		const string& posZ, 
		const string& negZ);
	GLuint ConvertSDLSurfaceToTexture(SDL_Surface *surf);
	GLuint ConvertSDLSurfaceToCubeMap(SDL_Surface *surf, int i, GLuint textId);

public:
	Texture(GLuint text) { textureId = text; }
	Texture(const string& fileName);
	Texture(
		const string& posX,
		const string& negX,
		const string& posY,
		const string& negY,
		const string& posZ,
		const string& negZ);
	~Texture();

	GLuint Get() { return textureId; }
};

#endif