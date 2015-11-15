#pragma once

#include "Common.h"
#include "Camera.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "GameObject.h"
#include "Model.h"
#include "Font.h"

class Game
{
private:
	Camera *camera;
	vector<GameObject*> gameObjects;
	vector<Texture*> textures;
	vector<ShaderProgram*> shaders;
	vector<Model*> models;
	Font *font;

	int fpsCounter = 0;
	int fpsDisplay = 0;
	float fpsTimer = 0;
	bool debugMode = false;

public:
	Game();
	~Game();

	void LoadResources();
	void ReleaseResources();

	void Update(float deltaTime);
	void Render();
};

