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

	static GLuint activeProg, activeText, activeVao;

public:
	Renderer();
	~Renderer();

	void AttachModel(Model *pModel) { model = pModel; }
	GLuint GetModel() { return model->Get(); }
	void AttachShaderProgram(ShaderProgram *sP) { shaderProg = sP; }
	GLuint GetProgram() { return shaderProg->Get(); }
	void SetTexture(Texture *pTexture) { texture = pTexture; }
	GLuint GetTexture() { return texture->Get(); }

	void Render(mat4 modelMat, Camera *cam);
};

#endif