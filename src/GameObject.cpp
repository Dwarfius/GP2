#include "GameObject.h"

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

void GameObject::Render(Camera *camera)
{
	if (!renderer)
		return;

	modelMatrix = translate(mat4(1), pos);
	modelMatrix = rotate(modelMatrix, radians(rotation.x), vec3(1, 0, 0));
	modelMatrix = rotate(modelMatrix, radians(rotation.y), vec3(0, 1, 0));
	modelMatrix = rotate(modelMatrix, radians(rotation.z), vec3(0, 0, 1));
	modelMatrix = scale(modelMatrix, size);

	renderer->Ready();

	ShaderProgram *program = renderer->GetProgram();
	program->SetUniform("Model", value_ptr(modelMatrix));
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

	renderer->Render(camera);
}

void GameObject::AttachComponent(BaseComponent * com)
{
	if (components.count(typeid(*com).name()) >= 1) {
		cout << "Component " << typeid(*com).name() << " is already attached to GO: " << name << endl;
		return;
	}
	
	Renderer *r = dynamic_cast<Renderer*>(com);
	if (r)
		renderer = r;

	components[typeid(*com).name()] = com;
	cout << typeid(*com).name() << endl;
	com->SetParentGO(this);
	cout << "Component " << typeid(*com).name() << " is attached to GO: " << name << endl;
}

BaseComponent * GameObject::GetComponent(string componentType)
{
	if(components.count("class " + componentType) == 0){
		cout << "Component "<< componentType <<" does not exist or is not attached to GO: " << name << endl;
		return NULL;
	}
	else {
		cout << "Component: " << componentType << " returned." << endl;
		return components.find("class " + componentType)->second;
	}
}

