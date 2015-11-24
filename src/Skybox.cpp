#include "Skybox.h"
#include "GameObject.h"

Skybox::Skybox(Texture *skybox, Model *model, ShaderProgram *shader, Texture *skyN, bool iTD)
{
	this->skybox = skybox;
	this->model = model;
	this->shader = shader;
	isTimeDay = iTD;
	if (isTimeDay) {
		skyNight = skyN;
	}
	else { skyNight = skybox; }
}

Skybox::~Skybox()
{
}

void Skybox::SetSkyNight(Texture * t)
{
	skyNight = t;
}

void Skybox::SetParentGO(GameObject *pGO)
{
	pGameObject = pGO;
	tD = dynamic_cast<TimeDay*>(pGameObject->GetComponent("TimeDay"));
	renderer = pGameObject->GetRenderer();
	renderer->SetTexture(0, skybox, true);
	if (isTimeDay) {
		renderer->SetTexture(1, skybox, true);
	}
	renderer->SetModel(model, GL_TRIANGLES);
	renderer->SetShaderProgram(shader);
}

void Skybox::Update(float deltaTime)
{
	if (isTimeDay) blendFactor = 10000000;
}

void Skybox::OnRender(Camera *camera)
{
	ShaderProgram *program = renderer->GetProgram();

	mat4 view = mat4(mat3(camera->GetView()));
	mat4 vp = camera->GetProj() * view;
	program->SetUniform("VP", value_ptr(vp));
	program->SetUniform("blendFactor", &blendFactor);
}