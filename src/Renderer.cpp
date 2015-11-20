#include "Renderer.h"
#include "Game.h"

GLuint Renderer::activeProg = 0, Renderer::activeVao = 0;
GLuint Renderer::activeTexts[TEXTURE_COUNT] = { 0, 0, 0, 0, 0 };

Renderer::Renderer(Texture *t, ShaderProgram *s, Model *m, int mode)
{
	AddTexture(t);
	shaderProg = s;
	model = m;
	renderMode = mode;
}

void Renderer::Render(Camera *cam)
{
	GLuint prog = shaderProg->Get();
	if (prog != activeProg)
	{
		glUseProgram(prog);
		activeProg = prog;
	}

	//and sending settings
	GLint loc;
	for (int i = 0; i < textCount; i++)
	{
		GLuint text = textures[i]->Get();
		if (text != activeTexts[i])
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, text);

			string name = string("texture") + (char)(48 + i);
			shaderProg->SetUniform(name, &i);

			activeTexts[i] = text;
		}
	}

	mat4 modelMat = pGameObject ? pGameObject->GetModelMatrix() : mat4(1);

	if (cam)
	{
		mat4 MVP = cam->Get() * modelMat;
		shaderProg->SetUniform("MVP", value_ptr(MVP));

		vec3 camPos = cam->GetPos();
		shaderProg->SetUniform("cameraPosition", &camPos);
	}

	shaderProg->SetUniform("Model", value_ptr(modelMat));

	vec3 lightDir(0, 0, 1);
	shaderProg->SetUniform("lightDirection", &lightDir);

	vec4 ambMatColor(0.3f, 0.3f, 0.3f, 1);
	shaderProg->SetUniform("ambientMaterialColor", &ambMatColor);

	vec4 difMatColor(1, 0, 0, 1);
	shaderProg->SetUniform("diffuseMaterialColor", &difMatColor);

	vec4 specMatColor(1, 1, 1, 1);
	shaderProg->SetUniform("specularMaterialColor", &specMatColor);

	float power = 25;
	shaderProg->SetUniform("specularPower", &power);

	vec4 ambLightColor(0.3f, 0.3f, 0.3f, 1);
	shaderProg->SetUniform("ambientLightColor", &ambLightColor);

	vec4 difLightColor(0.8f, 0.8f, 0.8f, 1);
	shaderProg->SetUniform("diffuseLightColor", &difLightColor);

	vec4 specLightColor(1, 1, 1, 1);
	shaderProg->SetUniform("specularLightColor", &specLightColor);
	
	//binding the vao
	GLuint vao = model->Get();
	if (vao != activeVao)
	{
		glBindVertexArray(vao);
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

	Game::verticesRendered += model->GetVertCount();
	Game::objectsRendered++;
	Game::drawCalls++;
}

//FINISH THIS UP
void Renderer::RenderInstanced(Camera *cam, int count)
{
	//binding the shader
	GLuint prog = shaderProg->Get();
	if (prog != activeProg)
	{
		glUseProgram(prog);
		activeProg = prog;
	}

	//and sending settings
	GLint loc;
	for (int i = 0; i < textCount; i++)
	{
		GLuint text = textures[i]->Get();
		if (text != activeTexts[i])
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, text);

			string name = string("texture") + (char)(48 + i);
			loc = glGetUniformLocation(shaderProg->Get(), name.c_str());
			glUniform1i(loc, i);

			activeTexts[i] = text;
		}
	}

	//binding the vao
	GLuint vao = model->Get();
	if (vao != activeVao)
	{
		glBindVertexArray(vao);
		activeVao = vao;
	}

	switch (renderMode)
	{
	case GL_TRIANGLES:
		glDrawElementsInstanced(GL_TRIANGLES, model->GetIndCount(), GL_UNSIGNED_INT, 0, count);
		break;
	case GL_TRIANGLE_FAN:
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, model->GetVertCount(), count);
		break;
	default:
		printf("Render mode unsupported!");
		break;
	}
	CHECK_GL_ERROR();

	Game::verticesRendered += model->GetVertCount() * count;
	Game::objectsRendered += count;
	Game::drawCalls++;
}