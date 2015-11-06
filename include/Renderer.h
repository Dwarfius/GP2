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
	Renderer() {}
	~Renderer() {}

	//mode supports either GL_TRIANGLE_FAN or GL_TRIANGLES
	void SetModel(Model *pModel, int mode) { model = pModel; renderMode = mode; }
	GLuint GetModel() { return model->Get(); }
	void SetShaderProgram(ShaderProgram *sP) { shaderProg = sP; }
	GLuint GetProgram() { return shaderProg->Get(); }
	void SetTexture(Texture *pTexture) { texture = pTexture; }
	GLuint GetTexture() { return texture ? texture->Get() : 0; }

	void Render(mat4 modelMat, Camera *cam);
};

#endif