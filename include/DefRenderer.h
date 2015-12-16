#ifndef _DEF_RENDERer_H
#define _DEF_RENDERer_H

#include "Common.h"
#include "Renderer.h"
#include "Camera.h"

class DefRenderer
{
private:
	static GLuint fbo;
	static GLuint rbo; //holds the depth and stencil

	//keeping track of model and shader
	static Texture *textures[3];
	static Model *model;
	static ShaderProgram *program;
	static ShaderProgram *nullProg;
	static Renderer *renderer;

	static vec3 sunDir;
	static vec3 sunColor;

	DefRenderer() {}
public:
	~DefRenderer() {}

	static void Init();
	static void CleanUp();
	//get the deferred renderer frame buffer
	static GLuint Get() { return fbo; }
	static void BeginGeomGather();
	static void EndGeomGather();
	static void BeginLightGather();
	static void EndLightGather();
	static void StencilPass(Camera *cam, Renderer *r);
	static void LightPass(Camera *cam, Renderer *r);
	static void RenderGather();

	static vec3 GetSunDir() { return sunDir; }
	static void SetSunDir(vec3 dir) { sunDir = dir; }

	static vec3 GetSunColor() { return sunColor; }
	static void SetSunColor(vec3 color) { sunColor = color; }
};

#endif