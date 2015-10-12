#include "Renderer.h"

Renderer::Renderer()
{
	model = new Model();
	shaderProg = new ShaderProgram(SHADER_PATH + "simpleVS.glsl", SHADER_PATH + "simpleFS.glsl");
}

Renderer::~Renderer()
{
	delete shaderProg;
	delete model;
}

void Renderer::Render(mat4 modelMat, mat4 vpMat)
{
	glBindVertexArray(model->Get());
	glUseProgram(shaderProg->Get());

	GLint loc = glGetUniformLocation(shaderProg->Get(), "MVP");
	mat4 MVP = vpMat * modelMat;
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(MVP));
	
	/*loc = glGetUniformLocation(shaderProg->Get(), "colorIn");
	glUniform3f(loc, color.r, color.g, color.b);*/

	glDrawElements(GL_TRIANGLES, model->Count(), GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();
}