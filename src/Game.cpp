#include "Game.h"
#include "Input.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::LoadResources()
{
	font = new Font(FONT_PATH + "OratorStd.otf");

	textures.push_back(new Texture(TEXTURE_PATH + "Tank1DF.png"));
	models.push_back(new Model(MODEL_PATH + "utah-teapot.FBX"));
	shaders.push_back(new ShaderProgram(SHADER_PATH + "specularVS.glsl", SHADER_PATH + "specularFS.glsl"));

	GameObject *cameraGameObject = new GameObject();
	camera = new Camera();
	CameraBehaviour *cb = new CameraBehaviour(camera);
	cameraGameObject->AttacheComponent(cb);
	gameObjects.push_back(cameraGameObject);

	//testing 10 teapots
	for (int i = 0; i < 10; i++)
	{
		GameObject *go = new GameObject();
		go->SetRotation(vec3(0, i * 36, 0));
		go->SetScale(vec3(0.1f, 0.1f, 0.1f));
		go->SetPos(vec3(i * 5, 0, 0));

		Renderer *renderer = new Renderer();
		renderer->SetTexture(textures[0]);
		renderer->AttachModel(models[0]);
		renderer->AttachShaderProgram(shaders[0]);

		go->SetRenderer(renderer);
		gameObjects.push_back(go);
	}
}

void Game::ReleaseResources()
{
	delete font;

	//clearing out all the objects
	int count = gameObjects.size();
	while (count-- > 0)
	{
		delete gameObjects[count];
		gameObjects.pop_back();
	}

	count = textures.size();
	while (count-- > 0)
	{
		delete textures[count];
		textures.pop_back();
	}

	count = shaders.size();
	while (count-- > 0)
	{
		delete shaders[count];
		shaders.pop_back();
	}

	count = models.size();
	while (count-- > 0)
	{
		delete models[count];
		models.pop_back();
	}

	delete camera;
}

void Game::Update(float deltaTime)
{
	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		(*iter)->AddRotation(vec3(0, deltaTime * 15, 0));
		(*iter)->Update(deltaTime);
	}

	if (Input::GetKeyDown(SDLK_k))
	{
		wireframeMode = !wireframeMode;
		if (wireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	fpsCounter++;
	if ((fpsTimer += deltaTime) > 1)
	{
		printf("FPS: %d\n", fpsCounter);
		fpsCounter = fpsTimer = 0;
	}
}

bool Comparer(GameObject *a, GameObject *b)
{
	Renderer *aRenderer = a->GetRenderer();
	Renderer *bRenderer = b->GetRenderer();
	if (!aRenderer) //all gameobjects without renderer go last
		return false;
	if (!bRenderer)
		return true;

	//first, sort by VAOs
	GLuint aVao = aRenderer->GetModel();
	GLuint bVao = bRenderer->GetModel();

	if (aVao == bVao)
	{
		//since we have same VAOs, sort by texture
		GLuint aText = aRenderer->GetTexture();
		GLuint bText = bRenderer->GetTexture();

		if (aText == bText) //since same texture, sort by shader
			return aRenderer->GetProgram() > bRenderer->GetProgram();
		else
			return aText > bText;
	}
	else
		return aVao > bVao;
}

void Game::Render()
{
	//premature optimization, but should help with large amounts of objects
	//sort the gameobjects for rendering to avoid extra calls to glBind of VAO/Texture/Shader
	sort(gameObjects.begin(), gameObjects.end(), Comparer);

	camera->Recalculate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
		(*iter)->Render(camera);
}