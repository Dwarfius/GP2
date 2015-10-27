#ifndef _RENDERER_H
#define _RENDERER_H

#include "Common.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Texture.h"
#include "BaseComponent.h"

class Renderer: public BaseComponent
{
private:
	Texture *texture;
	ShaderProgram *shaderProg;
	Model *model;

public:
	Renderer();
	~Renderer();

	void AttachModel(Model *pModel) { model = pModel; }
	void AttachShaderProgram(ShaderProgram *sP) { shaderProg = sP; }
	void SetTexture(Texture *pTexture) { texture = pTexture; }

	void Render(mat4 modelMat, mat4 vpMat);
};

#endif