#include "Renderer.h"
#include "Game.h"

GLuint Renderer::activeProg = 0, Renderer::activeVao = 0;
GLuint Renderer::activeTexts[TEXTURE_COUNT] = { 0, 0, 0, 0, 0 };

Renderer::Renderer(Texture *t, bool cubeMap, ShaderProgram *s, Model *m, int mode)
{
	AddTexture(t, cubeMap);
	shaderProg = s;
	model = m;
	renderMode = mode;
}

void Renderer::Ready()
{
	GLuint prog = shaderProg->Get();
	if (prog != activeProg)
	{
		glUseProgram(prog);
		activeProg = prog;
	}
}

void Renderer::Render()
{
	//and sending settings
	GLint loc;
	if (isCubeMap)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]->Get());

		int id = 0;
		shaderProg->SetUniform("skybox", &id);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textures[1]->Get());

		int id2 = 1;
		shaderProg->SetUniform("skyboxNight", &id2);
	}
	else
	{
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
	}

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
		printf("Render mode unsupported!\n");
		break;
	}
	CHECK_GL_ERROR();

	Game::verticesRendered += model->GetVertCount();
	Game::drawCalls++;
}

void Renderer::RenderInstanced(Camera *cam, int count)
{
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

	if (cam)
	{
		mat4 modelMat = pGameObject ? pGameObject->GetModelMatrix() : mat4(1);
		mat4 MVP = cam->Get() * modelMat;
		shaderProg->SetUniform("MVP", value_ptr(MVP));

		vec3 camPos = cam->GetPos();
		shaderProg->SetUniform("cameraPosition", &camPos);
	}
	if (!pGameObject)
	{
		mat4 model(1);
		shaderProg->SetUniform("Model", value_ptr(model));
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
	Game::drawCalls++;
}