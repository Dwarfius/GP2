#ifndef _GAME_H
#define _GAME_H

#include "Common.h"
#include "Camera.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "GameObject.h"
#include "Model.h"
#include "Font.h"
#include "SceneManager.h"

class Game
{
private:
	SceneManager * sceneManager;
	Scene * currentScene;
	ResourceManager* resourceManager;
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
	void Render(float deltaTime);

	static uint verticesRendered;
	static uint objectsRendered;
	static uint drawCalls;
};

#endif