#pragma once

#include "Common.h"
#include "Camera.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "GameObject.h"
#include "Model.h"
#include "CameraBehaviour.h"
#include "Font.h"
#include "SceneManager.h"

class Game
{
private:
	SceneManager * sceneManager;
	Scene * currentScene;
	Camera *camera;
	vector<GameObject*> gameObjects;
	vector<Texture*> textures;
	vector<ShaderProgram*> shaders;
	vector<Model*> models;
	Font *font;
	bool wireframeMode = false;

	int fpsCounter = 0;
	int fpsDisplay = 0;
	float fpsTimer = 0;

public:
	Game();
	~Game();

	void LoadResources();
	void ReleaseResources();

	void Update(float deltaTime);
	void Render();
};

