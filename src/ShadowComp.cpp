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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	int id = 0;
	s->SetUniform("shadowMap", &id);
	s->SetUniform("lightSpaceMatrix", value_ptr(LSM));
}

void ShadowComp::SetShadowUniforms(mat4 tLSM, GLuint tshadowMap)
{
	LSM = tLSM;
	shadowMap = tshadowMap;
}
