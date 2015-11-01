#include "Font.h"

Font::Font(const string& fileName)
{
	TTF_Font *font = TTF_OpenFont(fileName.c_str(), 64);
	if (!font)
	{
		printf("Unable to load font %s-%s\n", fileName.c_str(), TTF_GetError());
	}

	//first of all, need to render out the text atlas to use for vertex rendering
	SDL_Surface *atlasSurf = SDL_CreateRGBSurface(0, width, height, 32, 
								0xFF000000, 0xFF0000, 0xFF00, 0xFF);
	//to track where to put next glyphs
	int x = 0, y = 0;
	//marks where the newline starts
	int newY = 0; 
	//32 being space, 126 being ~, all visible characters
	for (uint glyph = 32; glyph < 127; glyph++)
	{
		SDL_Surface *glyphSurf = TTF_RenderGlyph_Blended(font, glyph, {255, 255, 255});
		int w = glyphSurf->w, h = glyphSurf->h;

		if (x + w > width) //if we can't go right anymore
		{
			x = 0; //start from left
			y += newY; //on a new line
			newY = 0; //start looking for a new newline offset
		}
		
		SDL_Rect r = { x, y, w, h };
		rects[glyph - 32] = r; //saving the unnormalized UVs
		SDL_BlitSurface(glyphSurf, NULL, atlasSurf, &r);
		x += w;
		newY = glm::max(newY, h);
		SDL_FreeSurface(glyphSurf);
	}

	//for debug
	SDL_SaveBMP(atlasSurf, "atlas.bmp");

	atlasText = ConvertSDLSurfaceToTexture(atlasSurf);
	SDL_FreeSurface(atlasSurf);
	TTF_CloseFont(font);

	//saving the previous state
	GLint currActive;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &currActive);

	//pushing our texture and setting standard settings
	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_2D, atlasText);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//restoring it
	glActiveTexture(currActive);
}

Font::~Font()
{
	glDeleteTextures(1, &atlasText);
}

void Font::Render(const string& text, const SDL_Rect rect)
{

}

GLuint Font::ConvertSDLSurfaceToTexture(SDL_Surface *surf)
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