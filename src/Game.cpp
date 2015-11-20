#include "Game.h"
#include "Input.h"
#include "PostProcessing.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::LoadResources()
{
	resourceManager = new ResourceManager();
	currentScene = new Scene(resourceManager);
	sceneManager = new SceneManager(resourceManager);

	PostProcessing::Init();
	font = new Font(FONT_PATH + "OratorStd.otf");
	ShaderProgram *s = new ShaderProgram(SHADER_PATH + "specularVS.glsl", SHADER_PATH + "specularFS.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->BindAttribLoc(3, "vertexNormal");
	s->Link();
	resourceManager->AddShader(s, "Specular");

	s = new ShaderProgram(SHADER_PATH + "postProcVS.glsl", SHADER_PATH + "colorCorr1.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->Link();
	resourceManager->AddShader(s, "PostProcess1");

	s = new ShaderProgram(SHADER_PATH + "postProcVS.glsl", SHADER_PATH + "colorCorr2.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->Link();
	resourceManager->AddShader(s, "PostProcess2");

	sceneManager->LoadSceneDirectories();
	sceneManager->LoadScene("Main", currentScene);

	GameObject *cameraGameObject = new GameObject();
	cameraGameObject->SetName("CameraBehaviourObject");
	camera = new Camera();
	cameraGameObject->AttachComponent(new CameraBehaviour(camera));
	currentScene->AddGamObject(cameraGameObject);
}

void Game::ReleaseResources()
{
	resourceManager->ReleaseResources();
	PostProcessing::CleanUp();
	delete font;
	delete camera;
}

void Game::Update(float deltaTime)
{
	currentScene->Update(deltaTime);

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
		fpsDisplay = fpsCounter;
		fpsCounter = fpsTimer = 0;
	}

	char fpsBuffer[30];
	sprintf(fpsBuffer, "FPS: %d", fpsDisplay);
	string fpsString(fpsBuffer);
	font->Render(fpsString, { 0, 0, 100, 40 });
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
	sort(currentScene->gameObjects.begin(), currentScene->gameObjects.end(), Comparer);

	camera->Recalculate();
	glBindFramebuffer(GL_FRAMEBUFFER, PostProcessing::Get());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentScene->Render(camera);

	PostProcessing::Pass(resourceManager->GetShader("PostProcess1"));
	PostProcessing::Pass(resourceManager->GetShader("PostProcess2")); //if you apply shader[1] again you should see the initial image
	PostProcessing::RenderResult();

	font->Flush();
}