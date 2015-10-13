#include "Renderer.h"

Renderer::Renderer()
{
	//texture = new Texture(TEXTURE_PATH + "texture.png");
	texture = new Texture(FONT_PATH + "OratorStd.otf", "Hello World");
	model = new Model();
	shaderProg = new ShaderProgram(SHADER_PATH + "simpleVS.glsl", SHADER_PATH + "simpleFS.glsl");
}

Renderer::~Renderer()
{
	delete texture;
	delete shaderProg;
	delete model;
}

void Renderer::Render(mat4 modelMat, mat4 vpMat)
{
	//binding the shader and sending settings
	glUseProgram(shaderProg->Get());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->Get());

	GLint loc = glGetUniformLocation(shaderProg->Get(), "MVP");
	mat4 MVP = vpMat * modelMat;
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(MVP));

	loc = glGetUniformLocation(shaderProg->Get(), "texture0");
	glUniform1i(loc, 0);
	
	//binding the vao
	glBindVertexArray(model->Get());

	//now on to draw stuff
	glDrawElements(GL_TRIANGLES, model->Count(), GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();
}