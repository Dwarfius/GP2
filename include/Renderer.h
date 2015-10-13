#ifndef _RENDERER_H
#define _RENDERER_H

#include "Common.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Texture.h"

class Renderer
{
private:
	Texture *texture;
	ShaderProgram *shaderProg;
	Model *model;

public:
	Renderer();
	~Renderer();

	void Render(mat4 modelMat, mat4 vpMat);
};

#endif