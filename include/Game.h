#pragma once

#include "Common.h"
#include "Camera.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "GameObject.h"
#include "Model.h"

class Game
{
private:
	Camera *camera;
	vector<GameObject*> gameObjects;
	vector<Texture*> textures;
	vector<ShaderProgram*> shaders;
	vector<Model*> models;
	bool wireframeMode = false;

public:
	Game();
	~Game();

	void LoadResources();
	void ReleaseResources();

	void Update(float deltaTime);
	void Render();
};

