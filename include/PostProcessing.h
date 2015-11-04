#ifndef _POST_PROCESSING
#define _POST_PROCESSING

#include "Common.h"
#include "Renderer.h"

//class which is used to apply postprocessing to the
//current frame
class PostProcessing
{
private:
	static GLuint FBODepthBuffer;
	static GLuint fbo;
	static Model *model;
	static ShaderProgram *program;
	static Renderer *renderer;
	static Texture *texture;

	PostProcessing() {}
public:
	~PostProcessing() {}

	static void Init();
	static GLuint Get() { return fbo; }
	static void RenderResult();
	static void CleanUp();
};

#endif