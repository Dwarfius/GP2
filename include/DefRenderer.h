#ifndef _DEF_RENDERer_H
#define _DEF_RENDERer_H

#include "Common.h"
#include "Renderer.h"

class DefRenderer
{
private:
	static GLuint fbo;

	//keeping track of model and shader
	static Texture *textures[3];
	static Model *model;
	static ShaderProgram *program;
	static Renderer *renderer;

	DefRenderer() {}
public:
	~DefRenderer() {}

	static void Init();
	static void CleanUp();
	//get the deferred renderer frame buffer
	static GLuint Get() { return fbo; }
	static void RenderGather();
};

#endif