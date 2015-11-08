#include "Renderer.h"

GLuint Renderer::activeProg = 0, Renderer::activeText = 0, Renderer::activeVao = 0;

Renderer::Renderer(Texture *t, ShaderProgram *s, Model *m, int mode)
{
	texture = t;
	shaderProg = s;
	model = m;
	renderMode = mode;
}

void Renderer::Render(mat4 modelMat, Camera *cam)
{
	//binding the shader
	GLuint prog = shaderProg->Get();
	if (prog != activeProg)
	{
		glUseProgram(prog);
		CHECK_GL_ERROR();
		activeProg = prog;
	}

	GLuint text = texture->Get();
	if (text != activeText)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, text);
		CHECK_GL_ERROR();
		activeText = text;
	}

	//and sending settings
	GLint loc;
	if (cam)
	{
		loc = glGetUniformLocation(shaderProg->Get(), "MVP");
		mat4 MVP = cam->Get() * modelMat;
		glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(MVP));
		CHECK_GL_ERROR();

		loc = glGetUniformLocation(shaderProg->Get(), "cameraPosition");
		vec3 camPos = cam->GetPos();
		glUniform3f(loc, camPos.x, camPos.y, camPos.z);
		CHECK_GL_ERROR();
	}

	loc = glGetUniformLocation(shaderProg->Get(), "Model");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(modelMat));
	CHECK_GL_ERROR();

	loc = glGetUniformLocation(shaderProg->Get(), "texture0");
	glUniform1i(loc, 0);

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
	GLuint vao = model->Get();
	if (vao != activeVao)
	{
		glBindVertexArray(vao);
		CHECK_GL_ERROR();
		activeVao = vao;
	}

	//now on to draw stuff
	switch (renderMode)
	{
	case GL_TRIANGLES:
		glDrawElements(GL_TRIANGLES, model->GetIndCount(), GL_UNSIGNED_INT, 0);
		break;
	case GL_TRIANGLE_FAN:
		glDrawArrays(GL_TRIANGLE_STRIP, 0, model->GetVertCount());
		break;
	default:
		printf("Render mode unsupported!");
		break;
	}
	CHECK_GL_ERROR();
}