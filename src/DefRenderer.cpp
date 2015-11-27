#include "DefRenderer.h"
#include "Graphics.h"

GLuint DefRenderer::fbo;
Texture* DefRenderer::textures[3];
Model* DefRenderer::model;
ShaderProgram* DefRenderer::program;
Renderer* DefRenderer::renderer;
vec3 DefRenderer::sunDir = vec3(0, -1, 0);
vec4 DefRenderer::sunColor = vec4(0.5f, 0.5f, 0.5f, 1);

void DefRenderer::Init()
{
	ivec2 screen = Graphics::GetViewport();

	glGenFramebuffers(1, &fbo);
	GLuint FBOtexture[3]; //color, normal, depth
	glGenTextures(3, FBOtexture);
	CHECK_GL_ERROR();
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	for (int i = 0; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, FBOtexture[i]);
		if(i == 2)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, screen.x, screen.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		else if (i == 1)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screen.x, screen.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screen.x, screen.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		textures[i] = new Texture(FBOtexture[i]);
	}
	CHECK_GL_ERROR();

	//configuring frame buffer
	//setting texture attachments
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtexture[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, FBOtexture[1], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FBOtexture[2], 0);
	CHECK_GL_ERROR();

	//marking that frag shader will render to the 2 bound textures
	GLenum bufferToDraw[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, bufferToDraw);
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

	program = new ShaderProgram(SHADER_PATH + "postProcVS.glsl", SHADER_PATH + "defRendFS.glsl");
	program->BindAttribLoc(0, "vertexPosition");
	program->Link();

	renderer = new Renderer();
	for (int i = 0; i < 3; i++)
		renderer->AddTexture(textures[i]);
	renderer->SetModel(model, GL_TRIANGLE_FAN);
	renderer->SetShaderProgram(program);
}

void DefRenderer::CleanUp()
{
	glDeleteFramebuffers(1, &fbo);
	delete program;
	delete model;
	for (int i = 0; i < 3; i++)
		delete textures[i];
	delete renderer;
}

void DefRenderer::RenderGather()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer->Ready();
	
	program->SetUniform("sunDir", &sunDir);
	program->SetUniform("sunDir", &sunDir);

	renderer->Render();
}