#include "Texture.h"

Texture::Texture(const string& fileName)
{
	textureId = LoadTextureFromFile(fileName);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(const string& font, const string& text)
{
	textureId = LoadTextureFromFont(font, 24, text);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
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

GLuint Texture::LoadTextureFromFont(const string& fileName, int size, const string& text)
{
	TTF_Font *font = TTF_OpenFont(fileName.c_str(), size);
	if (!font)
	{
		printf("Unable to load font %s-%s\n", fileName.c_str(), TTF_GetError());
		return 0;
	}

	SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), { 255, 255, 0 });
	GLuint textId = ConvertSDLSurfaceToTexture(surf);
	TTF_CloseFont(font);
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