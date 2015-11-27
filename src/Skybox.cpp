#include "Skybox.h"
#include "GameObject.h"
#include "DefRenderer.h"

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

void Skybox::SetParentGO(GameObject *pGO)
{
	pGameObject = pGO;
	tD = dynamic_cast<TimeDay*>(pGameObject->GetComponent("TimeDay"));
	renderer = pGameObject->GetRenderer();
	renderer->SetTexture(0, skybox, true);
	if (isTimeDay) {
		renderer->SetTexture(1, skyNight, true);
	}
	renderer->SetModel(model, GL_TRIANGLES);
	renderer->SetShaderProgram(shader);
}

void Skybox::Update(float deltaTime)
{
	if (isTimeDay)
	{
		bool nightTime = tD->GetHour() < 12;
		float k;
		if (nightTime)
		{
			k = ((tD->GetHour() + tD->GetMinute() / 60.f) / 12.f);
			blendFactor = 1 - k;
		}
		else
		{
			k = ((tD->GetHour() - 12 + tD->GetMinute() / 60.f) / 12.f);
			blendFactor = k;
		}
		
		//first, the sun direction
		//lerping - lerp(0, Pi, 1-k); - using 1-k cause blendFactor already inverted
		float angle = pi<float>() * blendFactor;
		vec3 dir(cos(angle), -sin(angle), 0);
		DefRenderer::SetSunDir(dir);

		//now, the color
		vec3 dayColor(1, 1, 1);
		vec3 nightColor = -dayColor;
		vec3 color;
		if (nightTime)
			color = mix(nightColor, dayColor, k);
		else
			color = mix(dayColor, nightColor, k);
		DefRenderer::SetSunColor(vec4(color, 1));
	}
}

void Skybox::OnRender(Camera *camera)
{
	ShaderProgram *program = renderer->GetProgram();

	mat4 mvp = camera->GetProj() * mat4(mat3(camera->GetView()));
	program->SetUniform("VP", value_ptr(mvp));
	program->SetUniform("blendFactor", &blendFactor);
}