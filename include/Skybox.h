#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "BaseComponent.h"
#include "Renderer.h"

class Skybox : 	public BaseComponent
{
private:
	Renderer *renderer;
	Texture *skybox;
	Model *model;
	ShaderProgram *shader;

public:
	Skybox(Texture *skybox, Model *model, ShaderProgram *shader);
	~Skybox();

	void SetParentGO(GameObject *pGO);

	void OnRender(Camera *camera);
};

#endif