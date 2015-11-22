#include "Texture.h"

Texture::Texture(const string& fileName)
{
	textureId = LoadTextureFromFile(fileName);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(const string & posX, const string & negX, const string & posY, const string & negY, const string & posZ, const string & negZ)
{
	textureId = LoadCubmapFromFiles(posX, negX, posY, negY, posZ, negZ);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);
}

GLuint Texture::LoadTextureFromFile(const string& fileName)
{
	SDL_Surface *surf = IMG_Load(fileName.c_str());
	if (!surf)
	{
		printf("Couldn't load image %s-%s\n", fileName.c_str(), IMG_GetError());
		return 0;
	}

	GLuint textId = ConvertSDLSurfaceToTexture(surf);
	SDL_FreeSurface(surf);

	return textId;
}

GLuint Texture::LoadCubmapFromFiles(const string & posX, const string & negX, const string & posY, const string & negY, const string & posZ, const string & negZ)
{
	string textureFaces[6];
	textureFaces[0] = posX;
	textureFaces[1] = negX;
	textureFaces[2] = posY;
	textureFaces[3] = negY;
	textureFaces[4] = posZ;
	textureFaces[5] = negZ;

	GLuint textId = 0;
	glGenTextures(1, &textId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textId);
	for (int i = 0; i < 6; i++) 
	{
		SDL_Surface *surf = IMG_Load(textureFaces[i].c_str());
		if (!surf)
		{
			printf("Couldn't load image %s-%s\n", textureFaces[i].c_str(), IMG_GetError());
			return 0;
		}

		ConvertSDLSurfaceToCubeMap(surf, i, textId);
		SDL_FreeSurface(surf);
	}
	return textId;
}

GLuint Texture::ConvertSDLSurfaceToTexture(SDL_Surface *surf)
{
	GLint nColors = surf->format->BytesPerPixel;
	GLenum textureFormat;
	GLenum internalFormat;

	if (nColors == 4) //contains alpha
	{
		if (surf->format->Rmask == 0xff)
			textureFormat = GL_RGBA;
		else
			textureFormat = GL_BGRA;
		internalFormat = GL_RGBA8;
	}
	else if (nColors == 3) //no alpha
	{
		if (surf->format->Rmask == 0xff)
			textureFormat = GL_RGB;
		else
			textureFormat = GL_BGR;
		internalFormat = GL_RGB8;
	}
	else //no idea
	{
		printf("Unsupported format!");
		return 0;
	}

	GLuint textId = 0;
	glGenTextures(1, &textId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textId);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surf->w, surf->h, 0, 
		textureFormat, GL_UNSIGNED_BYTE, surf->pixels);

	return textId;
}

void Texture::ConvertSDLSurfaceToCubeMap(SDL_Surface * surf, int i, GLuint textId)
{
	GLint nColors = surf->format->BytesPerPixel;
	GLenum textureFormat;
	GLenum internalFormat;

	if (nColors == 4) //contains alpha
	{
		if (surf->format->Rmask == 0xff)
			textureFormat = GL_RGBA;
		else
			textureFormat = GL_BGRA;
		internalFormat = GL_RGBA8;
	}
	else if (nColors == 3) //no alpha
	{
		if (surf->format->Rmask == 0xff)
			textureFormat = GL_RGB;
		else
			textureFormat = GL_BGR;
		internalFormat = GL_RGB8;
	}
	else //no idea
		printf("Cubemap %d - Unsupported format!", i);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, internalFormat, surf->w, surf->h, 0,
		textureFormat, GL_UNSIGNED_BYTE, surf->pixels);
}
