#ifndef _RENDERER_H
#define _RENDERER_H

#include "Common.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Texture.h"
#include "BaseComponent.h"
#include "Camera.h"

#define TEXTURE_COUNT 5

class Renderer: public BaseComponent
{
private:
	Texture* textures[TEXTURE_COUNT];
	ShaderProgram *shaderProg;
	Model *model;
	int renderMode = 0;
	int textCount = 0;

	static GLuint activeProg, activeVao;
	static GLuint activeTexts[TEXTURE_COUNT];

public:
	Renderer() { }
	Renderer(Texture *t, ShaderProgram *s, Model *m, int mode);
	~Renderer() {}

	//mode supports either GL_TRIANGLE_FAN or GL_TRIANGLES
	void SetModel(Model *pModel, int mode) { model = pModel; renderMode = mode; }
	Model* GetModel() { return model; }

	void SetShaderProgram(ShaderProgram *sP) { shaderProg = sP; }
	ShaderProgram* GetProgram() { return shaderProg; }

	void AddTexture(Texture *t) { textures[textCount++] = t; }
	void SetTexture(int i, Texture *t) { textures[i] = t; }
	Texture* GetTexture(int i) { return textures[i]; }

	void Render(mat4 modelMat, Camera *cam);
};

#endif