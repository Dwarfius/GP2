#include "GameObject.h"
#include "Game.h"

GameObject::GameObject()
{
	pos = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);
	size = vec3(1, 1, 1);
}

GameObject::~GameObject()
{
	if (!renderer)
		delete renderer;
}

void GameObject::Update(float deltaTime)
{
	for (auto it = components.begin(); it != components.end(); it++) {
		(*(it->second)).Update(deltaTime);
	}
}

void GameObject::UpdateMatrix()
{
	Model *m = renderer->GetModel();

	modelMatrix = translate(mat4(1), pos);
	modelMatrix = rotate(modelMatrix, radians(rotation.x), vec3(1, 0, 0));
	modelMatrix = rotate(modelMatrix, radians(rotation.y), vec3(0, 1, 0));
	modelMatrix = rotate(modelMatrix, radians(rotation.z), vec3(0, 0, 1));
	modelMatrix = scale(modelMatrix, size);
	modelMatrix = translate(modelMatrix, -m->GetCenter());

	modelDirty = false;
}

void GameObject::Render(Camera *camera)
{
	if (!renderer)
		return;

	renderer->Ready();
	ShaderProgram *program = renderer->GetProgram();
	program->SetUniform("Model", value_ptr(modelMatrix));
	mat4 VP = camera->Get();
	program->SetUniform("VP", value_ptr(VP));
	mat4 MVP = VP * GetModelMatrix();
	program->SetUniform("MVP", value_ptr(MVP));

	vec3 camPos = camera->GetPos();
	program->SetUniform("cameraPosition", &camPos);

	vec3 lightDir(0, 0, 1);
	program->SetUniform("lightDirection", &lightDir);

	vec4 ambMatColor(1, 0, 0, 1);
	program->SetUniform("ambientMaterialColor", &ambMatColor);

	vec4 difMatColor(1, 0, 0, 1);
	program->SetUniform("diffuseMaterialColor", &difMatColor);

	vec4 specMatColor(1, 1, 1, 1);
	program->SetUniform("specularMaterialColor", &specMatColor);

	float power = 25;
	program->SetUniform("specularPower", &power);

	vec4 ambLightColor(1, 0, 0, 1);
	program->SetUniform("ambientLightColor", &ambLightColor);

	vec4 difLightColor(1, 0, 0, 1);
	program->SetUniform("diffuseLightColor", &difLightColor);

	vec4 specLightColor(1, 1, 1, 1);
	program->SetUniform("specularLightColor", &specLightColor);

	for (auto it = components.begin(); it != components.end(); it++)
		(*(it->second)).OnRender(camera);

	renderer->Render();
}

void GameObject::AttachComponent(BaseComponent * com)
{
	if (components.count(typeid(*com).name()) >= 1) 
		return;
	
	//caching
	Renderer *r = dynamic_cast<Renderer*>(com);
	if (r)
		renderer = r;

	Light *l = dynamic_cast<Light*>(com);
	if (l)
		light = l;

	components[typeid(*com).name()] = com;
	com->SetParentGO(this);
}

BaseComponent * GameObject::GetComponent(string componentType)
{
	if(components.count("class " + componentType) == 0)
		return NULL;
	else 
		return components.find("class " + componentType)->second;
}

