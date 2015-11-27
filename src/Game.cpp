#include "Game.h"
#include "Input.h"
#include "PostProcessing.h"
#include "CameraBehaviour.h"
#include "TerrainComp.h"
#include "DefRenderer.h"

uint Game::verticesRendered;
uint Game::objectsRendered;
uint Game::drawCalls;

vector<Vertex> skyBoxverts = {
	//Front
	{ vec3(-1, 1, 1),
	vec4(1.0f, 0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f) },// Top Left

	{ vec3(-1, -1, 1),
	vec4(1.0f, 1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f) },// Bottom Left

	{ vec3(1, -1, 1),
	vec4(0.0f, 1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f) }, //Bottom Right

	{ vec3(1, 1, 1),
	vec4(1.0f, 0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f) },// Top Right

													 //back
	{ vec3(-1, 1, -1),
	vec4(1.0f, 0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f) },// Top Left

	{ vec3(-1, -1, -1),
	vec4(1.0f, 1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f) },// Bottom Left

	{ vec3(1, -1, -1),
	vec4(0.0f, 1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f) }, //Bottom Right

	{ vec3(1, 1, -1),
	vec4(1.0f, 0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f) },// Top Right
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
	resourceManager->AddTexture("Tank1DF.png");
	resourceManager->AddTexture("grass.png");
	resourceManager->AddTexture("ground.jpg");
	resourceManager->AddTexture("rock.jpg");
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

	//========================  SHADERS ========================
	ShaderProgram *s = new ShaderProgram(SHADER_PATH + "specularVS.glsl", SHADER_PATH + "specularFS.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->BindAttribLoc(3, "vertexNormal");
	s->Link();
	resourceManager->AddShader(s, "Specular");

	s = new ShaderProgram(SHADER_PATH + "normalMappingVS.glsl", SHADER_PATH + "normalMappingFS.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->BindAttribLoc(2, "vertexTexCoords");
	s->BindAttribLoc(3, "vertexNormal");
	s->BindAttribLoc(4, "vertexTangent");
	s->BindAttribLoc(5, "vertexBinormal");
	s->Link();
	resourceManager->AddShader(s, "NormalMapping");

	s = new ShaderProgram(SHADER_PATH + "diffuseNormalSpecMapVS.glsl", SHADER_PATH + "diffuseNormalSpecMapFS.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->BindAttribLoc(2, "vertexTexCoords");
	s->BindAttribLoc(3, "vertexNormal");
	s->BindAttribLoc(4, "vertexTangent");
	s->BindAttribLoc(5, "vertexBinormal");
	s->Link();
	resourceManager->AddShader(s, "diffuseNormalSpecMap");

	s = new ShaderProgram(SHADER_PATH + "postProcVS.glsl", SHADER_PATH + "colorCorr1.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->Link();
	resourceManager->AddShader(s, "PostProcess1");

	s = new ShaderProgram(SHADER_PATH + "postProcVS.glsl", SHADER_PATH + "colorCorr2.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->Link();
	resourceManager->AddShader(s, "PostProcess2");

	s = new ShaderProgram(SHADER_PATH + "terrainVS.glsl", SHADER_PATH + "terrainFS.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->BindAttribLoc(1, "colors");
	s->BindAttribLoc(2, "uvs");
	s->BindAttribLoc(3, "normals");
	s->Link();
	resourceManager->AddShader(s, "Terrain");

	s = new ShaderProgram(SHADER_PATH + "skyboxVS.glsl", SHADER_PATH + "skyboxFS.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->Link();
	resourceManager->AddShader(s, "SkyBox");

	//======================== SCENEMANAGEMENT  ====================
	sceneManager->LoadSceneDirectories();
	sceneManager->LoadScene("Main", currentScene);
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
 	drawCalls = verticesRendered = objectsRendered = 0;

	//premature optimization, but should help with large amounts of objects
	//sort the gameobjects for rendering to avoid extra calls to glBind of VAO/Texture/Shader
	sort(currentScene->gameObjects.begin(), currentScene->gameObjects.end(), Comparer);

	currentScene->GetSceneCamera()->Recalculate();

	glBindFramebuffer(GL_FRAMEBUFFER, DefRenderer::Get());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentScene->Render(currentScene->GetSceneCamera());

	if (debugMode)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else
		glBindFramebuffer(GL_FRAMEBUFFER, PostProcessing::Get());

	DefRenderer::RenderGather();

	if (!debugMode)
	{
		//PostProcessing::Pass(shaders[1]);
		//PostProcessing::Pass(shaders[2]); //if you apply shader[1] again you should see the initial image
		PostProcessing::RenderResult();
	}

	char *msg = (char*)malloc(50);
	sprintf(msg, "verts:%u", verticesRendered);
	Font* tF = resourceManager->GetFont("OratorStd.otf");
	tF->Render(string(msg), { 0, 25, 100, 25 });
	sprintf(msg, "objts:%u(%u)", objectsRendered - 1, currentScene->gameObjects.size());
	tF->Render(string(msg), { 0, 50, 100, 25 });
	free(msg);

	resourceManager->FlushFonts(deltaTime);
}