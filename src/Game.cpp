#include "Game.h"
#include "Input.h"
#include "PostProcessing.h"
#include "CameraBehaviour.h"
#include "TerrainComp.h"
#include "DefRenderer.h"

uint Game::verticesRendered;
uint Game::drawCalls;
Scene* Game::currentScene;
ResourceManager* Game::resourceManager;

#define SKYBOX_DIST 5

vector<Vertex> skyBoxverts = {
	//Front
	{ vec3(-SKYBOX_DIST, SKYBOX_DIST, SKYBOX_DIST),
	vec4(0), vec2(0.0f, 0.0f) },// Top Left

	{ vec3(-SKYBOX_DIST, -SKYBOX_DIST, SKYBOX_DIST),
	vec4(0), vec2(0.0f, 1.0f) },// Bottom Left

	{ vec3(SKYBOX_DIST, -SKYBOX_DIST, SKYBOX_DIST),
	vec4(0), vec2(1.0f, 1.0f) }, //Bottom Right

	{ vec3(SKYBOX_DIST, SKYBOX_DIST, SKYBOX_DIST),
	vec4(0), vec2(1.0f, 0.0f) },// Top Right

	//Back
	{ vec3(-SKYBOX_DIST, SKYBOX_DIST, -SKYBOX_DIST),
	vec4(0), vec2(0.0f, 0.0f) },// Top Left

	{ vec3(-SKYBOX_DIST, -SKYBOX_DIST, -SKYBOX_DIST),
	vec4(0), vec2(0.0f, 1.0f) },// Bottom Left

	{ vec3(SKYBOX_DIST, -SKYBOX_DIST, -SKYBOX_DIST),
	vec4(0), vec2(1.0f, 1.0f) }, //Bottom Right

	{ vec3(SKYBOX_DIST, SKYBOX_DIST, -SKYBOX_DIST),
	vec4(0), vec2(1.0f, 0.0f) },// Top Right
};

vector<int> skyBoxIndices = {
	//front
	3, 2, 1,
	3, 1, 0,

	//left
	0, 1, 5,
	0, 5, 4,

	//right
	7, 6, 2,
	7, 2, 3,

	//bottom
	5, 1, 2,
	5, 2, 6,

	//top
	0, 4, 7,
	0, 7, 3,

	//back
	4, 5, 6,
	4, 6, 7
};

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

	DefRenderer::Init();
	PostProcessing::Init();
	CHECK_GL_ERROR();

	//======================== TEXTURES ========================
	//HIDE THIS AWAY IN TO SCENE MANAGER LOADER AT SOME POINT!
	Texture* skyTexture = new Texture(TEXTURE_PATH + "right.jpg", TEXTURE_PATH + "left.jpg", TEXTURE_PATH + "top.jpg", TEXTURE_PATH + "bottom.jpg", TEXTURE_PATH + "back.jpg", TEXTURE_PATH + "front.jpg");
	resourceManager->AddTexture("skyTexture", skyTexture);
	Texture* skyNightTexture = new Texture(TEXTURE_PATH + "night_right.jpg", TEXTURE_PATH + "night_left.jpg", TEXTURE_PATH + "night_top.jpg", TEXTURE_PATH + "night_bottom.jpg", TEXTURE_PATH + "night_back.jpg", TEXTURE_PATH + "night_front.jpg");
	resourceManager->AddTexture("skyNightTexture", skyNightTexture);

	//========================  MODELS  ========================
	Model *terrainModel = new Model();
	terrainModel->SetUpAttrib(0, 3, GL_FLOAT, 0); //pos
	terrainModel->SetUpAttrib(1, 4, GL_FLOAT, sizeof(vec3)); //coeffs
	terrainModel->SetUpAttrib(2, 2, GL_FLOAT, sizeof(vec3) + sizeof(vec4)); //uv
	terrainModel->SetUpAttrib(3, 3, GL_FLOAT, sizeof(vec3) + sizeof(vec4) + sizeof(vec2)); //normals
	resourceManager->AddModel("Terrain", terrainModel);

	Model *skyModel = new Model();
	vector<Vertex> *verts = new vector<Vertex>(skyBoxverts);
	vector<int> *indcs = new vector<int>(skyBoxIndices);
	skyModel->SetVertices(verts, GL_STATIC_DRAW, true);
	skyModel->SetIndices(indcs, GL_STATIC_DRAW, true);
	skyModel->FlushBuffers();
	skyModel->SetUpAttrib(0, 3, GL_FLOAT, 0);
	resourceManager->AddModel("skyModel", skyModel);

	//======================== SCENEMANAGEMENT  ====================
	sceneManager->LoadSceneDirectories();
	sceneManager->LoadScene(0, currentScene);
}

void Game::ReleaseResources()
{
	resourceManager->ReleaseResources();
	PostProcessing::CleanUp();
}

void Game::Update(float deltaTime)
{
	currentScene->Update(deltaTime);

	if (Input::GetKeyDown(SDLK_k))
	{
		debugMode = !debugMode;
		if (debugMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (Input::GetKeyDown(SDLK_b))
		negMode = !negMode;

	fpsCounter++;
	if ((fpsTimer += deltaTime) > 1)
	{
		fpsDisplay = fpsCounter;
		fpsCounter = fpsTimer = 0;
	}

	char fpsBuffer[30];
	sprintf(fpsBuffer, "FPS: %d", fpsDisplay);
	string fpsString(fpsBuffer);
	resourceManager->GetFont("OratorStd.otf")->Render(fpsString, { 0, 0, 100, 25 });
}

bool Comparer(GameObject *a, GameObject *b)
{
	if (a->GetComponent("SkyBox")) //skybox should be rendered last
		return false;
	if (b->GetComponent("SkyBox")) //so that depth testing has bigger impact
		return true;

	Renderer *aRenderer = a->GetRenderer();
	Renderer *bRenderer = b->GetRenderer();
	if (!aRenderer) //all gameobjects without renderer go last
		return false;
	if (!bRenderer)
		return true;

	//first, sort by VAOs
	Model *aModel = aRenderer->GetModel();
	Model *bModel = bRenderer->GetModel();
	if (!aModel)
		return false;
	if (!bModel)
		return true;
	GLuint aVao = aModel->Get();
	GLuint bVao = bModel->Get();

	if (aVao == bVao)
		return aRenderer->GetProgram() > bRenderer->GetProgram();
	else
		return aVao > bVao;
}

void Game::Render(float deltaTime)
{
 	drawCalls = verticesRendered = 0;

	Camera *camera = currentScene->GetSceneCamera();
	camera->Recalculate();
	currentScene->VisibilityCheck();
	currentScene->Sort(Comparer);

	if (debugMode)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentScene->Render(camera);
	}
	else
	{
		DefRenderer::BeginGeomGather();
		currentScene->Render(camera);
		DefRenderer::EndGeomGather();

		DefRenderer::BeginLightGather();
		int count = currentScene->GetLightCount();
		for (int i = 0; i < count; i++)
		{
			Renderer *r = currentScene->GetLight(i);
			DefRenderer::StencilPass(camera, r);
			DefRenderer::LightPass(camera, r);
		}
		DefRenderer::EndLightGather();

		glBindFramebuffer(GL_FRAMEBUFFER, PostProcessing::Get());

		DefRenderer::RenderGather();

		if(negMode)
			PostProcessing::Pass(resourceManager->GetShader("PostProcess1"));
		PostProcessing::RenderResult();
	}

	char *msg = (char*)malloc(50);
	sprintf(msg, "verts:%u", verticesRendered);
	Font* tF = resourceManager->GetFont("OratorStd.otf");
	tF->Render(string(msg), { 0, 25, 100, 25 });
	sprintf(msg, "objts:%u(%u)", currentScene->GetVisibleGOCount(), currentScene->GetGOCount());
	tF->Render(string(msg), { 0, 50, 100, 25 });
	free(msg);

	resourceManager->FlushFonts(deltaTime);
}