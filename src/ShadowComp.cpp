#include "ShadowComp.h"
#include "GameObject.h"


ShadowComp::ShadowComp()
{
}


ShadowComp::~ShadowComp()
{
}

void ShadowComp::Update(float deltaTime)
{

}

void ShadowComp::OnRender(Camera * camera)
{
	Renderer *r = dynamic_cast<Renderer*>(pGameObject->GetComponent("Renderer"));
	ShaderProgram *s = r->GetProgram();
	GLint temp = 35;
	s->SetUniform("shadowMap", &temp);  
	s->SetUniform("lightSpaceMatrix", value_ptr(LSM));
}

void ShadowComp::SetShadowUniforms(mat4 tLSM, GLuint tshadowMap)
{
	LSM = tLSM;
	shadowMap = tshadowMap;
}
