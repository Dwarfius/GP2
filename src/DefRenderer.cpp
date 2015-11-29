#include "DefRenderer.h"
#include "Graphics.h"
#include "GameObject.h"

GLuint DefRenderer::fbo;
Texture* DefRenderer::textures[3];
Model* DefRenderer::model;
ShaderProgram* DefRenderer::program;
ShaderProgram* DefRenderer::nullProg;
Renderer* DefRenderer::renderer;
vec3 DefRenderer::sunDir = vec3(0, -1, 0);
vec4 DefRenderer::sunColor = vec4(0.5f, 0.5f, 0.5f, 1);

void DefRenderer::Init()
{
	nullProg = new ShaderProgram(SHADER_PATH + "nullVS.glsl", SHADER_PATH + "nullFS.glsl");
	nullProg->BindAttribLoc(0, "Position");
	nullProg->Link();
	ivec2 screen = Graphics::GetViewport();

	glGenFramebuffers(1, &fbo);
	GLuint FBOtexture[3]; //color, normal, depth
	glGenTextures(3, FBOtexture);
	CHECK_GL_ERROR();
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	for (int i = 0; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, FBOtexture[i]);
		if(i == 2) //depth texture requires a special format
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, screen.x, screen.y, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
		else //everything else is standard
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
	//depth is handled in a different pipeline stage - no need to bother about it
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
	model->FlushBuffers();
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
	delete nullProg;
	delete model;
	for (int i = 0; i < 3; i++)
		delete textures[i];
	delete renderer;
}

//implementation of deferred shading
//http://ogldev.atspace.co.uk/www/tutorial37/tutorial37.html
void DefRenderer::BeginGeomGather()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DefRenderer::EndGeomGather()
{
	glDepthMask(GL_FALSE);
}

void DefRenderer::BeginLightGather()
{
	glEnable(GL_STENCIL_TEST);
}

void DefRenderer::EndLightGather()
{
	glDisable(GL_STENCIL_TEST);
}

//creates a stencil which will determine how light is applied to fragments
//needs to be called before every light pass to ensure proper work
void DefRenderer::StencilPass(Camera *cam, Renderer *r)
{
	ShaderProgram *temp = r->GetProgram();
	r->SetShaderProgram(nullProg);

	//only updating the stencil - no fragments drawn
	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);

	//all the fragments need to be processed by the rasterizer
	glDisable(GL_CULL_FACE);

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, 0, 0);

	//if light is blocked - stencil 0
	//if we pass through the light's front face and back face - stencil will be 0
	//otherwise, something in the light area blocked from leaving the area, so it's 1
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	r->Ready();
	mat4 mvp = cam->Get() * r->GetParentGO()->GetModelMatrix();
	nullProg->SetUniform("MVP", value_ptr(mvp));
	r->Render();
	r->SetShaderProgram(temp);
}

//applying the light at the "stencil-approved" areas
void DefRenderer::LightPass(Camera *cam, Renderer *r)
{
	//setting to pass the stencil test when != 0
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	//writing only on top of the colors from geometry pass
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	
	glDisable(GL_DEPTH_TEST);
	
	//using additive blending to mix the light and scene fragments
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	r->Ready();
	mat4 mvp = cam->Get() * r->GetParentGO()->GetModelMatrix();
	nullProg->SetUniform("MVP", value_ptr(mvp));
	vec4 color(1, 1, 1, 1); //TEMPRORARY VALUE
	r->GetProgram()->SetUniform("Color", &color);
	r->Render();

	glCullFace(GL_BACK);

	glDisable(GL_BLEND);
}

void DefRenderer::RenderGather()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer->Ready();
	
	program->SetUniform("sunDir", &sunDir);
	program->SetUniform("sunColor", &sunColor);

	renderer->Render();
}