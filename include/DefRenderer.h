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

	static vec3 sunDir;
	static vec4 sunColor;

	DefRenderer() {}
public:
	~DefRenderer() {}

	static void Init();
	static void CleanUp();
	//get the deferred renderer frame buffer
	static GLuint Get() { return fbo; }
	static void RenderGather();

	static vec3 GetSunDir() { return sunDir; }
	static void SetSunDir(vec3 dir) { sunDir = dir; }

	static vec4 GetSunColor() { return sunColor; }
	static void SetSunColor(vec4 color) { sunColor = color; }
};

#endif