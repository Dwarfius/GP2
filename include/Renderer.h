#ifndef _RENDERER_H
#define _RENDERER_H

#include "Common.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Texture.h"
#include "BaseComponent.h"
#include "Camera.h"

class Renderer: public BaseComponent
{
private:
	Texture *texture;
	ShaderProgram *shaderProg;
	Model *model;
	int renderMode = 0;

	static GLuint activeProg, activeText, activeVao;

public:
	Renderer() { }
	Renderer(Texture *t, ShaderProgram *s, Model *m, int mode);
	~Renderer() {}

	//mode supports either GL_TRIANGLE_FAN or GL_TRIANGLES
	void SetModel(Model *pModel, int mode) { model = pModel; renderMode = mode; }
	Model* GetModel() { return model; }
	void SetShaderProgram(ShaderProgram *sP) { shaderProg = sP; }
	ShaderProgram* GetProgram() { return shaderProg; }
	void SetTexture(Texture *pTexture) { texture = pTexture; }
	Texture* GetTexture() { return texture; }

	void Render(mat4 modelMat, Camera *cam);
};

#endif