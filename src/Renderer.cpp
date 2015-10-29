#include "Renderer.h"

Renderer::Renderer()
{
	//texture = new Texture(TEXTURE_PATH + "texture.png");
	//texture = new Texture(FONT_PATH + "OratorStd.otf", "Hello World");
	//shaderProg = new ShaderProgram(SHADER_PATH + "simpleVS.glsl", SHADER_PATH + "simpleFS.glsl");
}

Renderer::~Renderer()
{
	//delete texture;
	//delete shaderProg;
	//delete model;
}

void Renderer::Render(mat4 modelMat, Camera *cam)
{
	//binding the shader and sending settings
	glUseProgram(shaderProg->Get());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->Get());

	GLint loc = glGetUniformLocation(shaderProg->Get(), "MVP");
	mat4 MVP = cam->Get() * modelMat;
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(MVP));
	CHECK_GL_ERROR();

	loc = glGetUniformLocation(shaderProg->Get(), "Model");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(modelMat));
	CHECK_GL_ERROR();

	//loc = glGetUniformLocation(shaderProg->Get(), "texture0");
	//glUniform1i(loc, 0);

	loc = glGetUniformLocation(shaderProg->Get(), "cameraPosition");
	vec3 camPos = cam->GetPos();
	glUniform3f(loc, camPos.x, camPos.y, camPos.z);
	CHECK_GL_ERROR();

	loc = glGetUniformLocation(shaderProg->Get(), "lightDirection");
	glUniform3f(loc, 0, 0, 1);
	CHECK_GL_ERROR();

	loc = glGetUniformLocation(shaderProg->Get(), "ambientMaterialColor");
	glUniform4f(loc, 0.3f, 0.3f, 0.3f, 1);
	CHECK_GL_ERROR();

	loc = glGetUniformLocation(shaderProg->Get(), "diffuseMaterialColor");
	glUniform4f(loc, 1, 0, 0, 1);
	CHECK_GL_ERROR();

	loc = glGetUniformLocation(shaderProg->Get(), "specularMaterialColor");
	glUniform4f(loc, 1, 1, 1, 1);
	CHECK_GL_ERROR();

	loc = glGetUniformLocation(shaderProg->Get(), "specularPower");
	glUniform1f(loc, 25);
	CHECK_GL_ERROR();

	loc = glGetUniformLocation(shaderProg->Get(), "ambientLightColor");
	glUniform4f(loc, 0.3f, 0.3f, 0.3f, 1);
	CHECK_GL_ERROR();

	loc = glGetUniformLocation(shaderProg->Get(), "diffuseLightColor");
	glUniform4f(loc, 0.8f, 0.8f, 0.8f, 1);
	CHECK_GL_ERROR();

	loc = glGetUniformLocation(shaderProg->Get(), "specularLightColor");
	glUniform4f(loc, 1, 1, 1, 1);
	CHECK_GL_ERROR();
	
	//binding the vao
	glBindVertexArray(model->Get());
	CHECK_GL_ERROR();

	//now on to draw stuff
	glDrawElements(GL_TRIANGLES, model->GetIndCount(), GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();
}