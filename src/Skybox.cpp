#include "Skybox.h"
#include "GameObject.h"

Skybox::Skybox(Texture *skybox, Model *model, ShaderProgram *shader)
{
	this->skybox = skybox;
	this->model = model;
	this->shader = shader;
}

Skybox::~Skybox()
{
}

void Skybox::SetParentGO(GameObject *pGO)
{
	pGameObject = pGO;
	renderer = pGameObject->GetRenderer();
	renderer->SetTexture(0, skybox, true);
	renderer->SetModel(model, GL_TRIANGLES);
	renderer->SetShaderProgram(shader);
}

void Skybox::OnRender(Camera *camera)
{
	ShaderProgram *program = renderer->GetProgram();

	mat4 mvp = camera->GetProj() * mat4(mat3(camera->GetView()));
	program->SetUniform("VP", (void*)value_ptr(mvp));
}