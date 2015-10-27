#include "Game.h"
#include "Input.h"

Game::Game()
{
}

Game::~Game()
{
}
	GameObject *go = new GameObject();

void Game::LoadResources()
{
	camera = new Camera();
	CameraBehaviour *cb = new CameraBehaviour(camera);
	go->AttacheComponent(cb);
	Renderer *renderer = new Renderer();
	Texture *texture = new Texture(TEXTURE_PATH + "Tank1DF.png");
	Model *model = new Model(MODEL_PATH + "Tank1.FBX");
	renderer->SetTexture(texture);
	renderer->AttachModel(model);
	ShaderProgram *shader = new ShaderProgram(SHADER_PATH + "simpleVS.glsl", SHADER_PATH + "simpleFS.glsl");
	renderer->AttachShaderProgram(shader);
	go->SetRenderer(renderer);
	gameObjects.push_back(go);
}

void Game::ReleaseResources()
{
	delete camera;
}

void Game::Update(float deltaTime)
{
	go->Update(deltaTime);
	/*vec3 forward = camera->GetForward();
	vec3 right = camera->GetRight();
	if (Input::GetKey(SDLK_w))
		camera->Translate(forward * deltaTime);
	if (Input::GetKey(SDLK_s))
		camera->Translate(-forward * deltaTime);
	if (Input::GetKey(SDLK_d))
		camera->Translate(right * deltaTime);
	if (Input::GetKey(SDLK_a))
		camera->Translate(-right * deltaTime);*/

	if (Input::GetKeyDown(SDLK_k))
	{
		if (wireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		wireframeMode = !wireframeMode;
	}

	/*if (Input::GetMouseBtnDown(SDL_BUTTON_LEFT))
	printf("Left Down!\n");
	if (Input::GetMouseBtn(SDL_BUTTON_LEFT))
	printf("Left held down!\n");
	if (Input::GetMouseBtnUp(SDL_BUTTON_LEFT))
	printf("Left Up!\n");

	ivec2 deltaPos = Input::GetMouseDelta();
	camera->Rotate((float)deltaPos.x * deltaTime, (float)-deltaPos.y * deltaTime);*/
}

void Game::Render()
{
	camera->Recalculate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int count = gameObjects.size();
	for (int i = 0; i < count; i++)
		gameObjects[i]->Render(camera);
}
