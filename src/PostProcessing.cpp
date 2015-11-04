#include "PostProcessing.h"
#include "Graphics.h"

GLuint PostProcessing::FBODepthBuffer;
GLuint PostProcessing::fbo;
Model *PostProcessing::model;
ShaderProgram *PostProcessing::program;
Renderer *PostProcessing::renderer;
Texture *PostProcessing::texture;

void PostProcessing::Init()
{
	ivec2 screen = Graphics::GetViewport();

	//creating all the required objects
	//creating the frame buffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	CHECK_GL_ERROR();
	
	//texture for our frame buffer
	GLuint FBOtexture;
	glGenTextures(1, &FBOtexture);
	glBindTexture(GL_TEXTURE_2D, FBOtexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screen.x, screen.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	CHECK_GL_ERROR();

	//depth buffer for our frame buffer
	glGenRenderbuffers(1, &FBODepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, FBODepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, screen.x, screen.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FBODepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	CHECK_GL_ERROR();

	//configuring frame buffer
	//setting FBOtexture as color attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,	FBODepthBuffer);
	CHECK_GL_ERROR();

	//marking that frag shader will render to the bound buffer
	GLenum bufferToDraw[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, bufferToDraw); 
	CHECK_GL_ERROR();
	
	//check if we succeeded
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Error - Framebuffer incomplete!\n");

	//now create all the required resources for rendering the screen quad
	vector<Vertex> *verts = new vector<Vertex>();
	verts->push_back(Vertex(vec3(-1.f, -1.f, 0)));
	verts->push_back(Vertex(vec3(1.f, -1.f, 0)));
	verts->push_back(Vertex(vec3(-1.f, 1.f, 0)));
	verts->push_back(Vertex(vec3(1.f, 1.f, 0)));

	model = new Model();
	model->SetVertices(verts, GL_STATIC_DRAW, true);
	model->SetUpAttrib(0, 2, GL_FLOAT, 0); //vec2 position

	program = new ShaderProgram(SHADER_PATH + "postProcVS.glsl", SHADER_PATH + "postProcFS.glsl");
	program->BindAttribLoc(0, "vertexPosition");
	program->Link();

	texture = new Texture(FBOtexture);

	renderer = new Renderer();
	renderer->SetTexture(texture);
	renderer->SetModel(model, GL_TRIANGLE_FAN);
	renderer->SetShaderProgram(program);
}

void PostProcessing::CleanUp()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &FBODepthBuffer);
	delete program;
	delete model;
	delete texture;
	delete renderer;
}

void PostProcessing::RenderResult()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer->Render(mat4(1), NULL);
}