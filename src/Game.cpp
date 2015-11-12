#include "Game.h"
#include "Input.h"
#include "PostProcessing.h"
#include "CameraBehaviour.h"
#include "TerrainComp.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::LoadResources()
{
	PostProcessing::Init();
	font = new Font(FONT_PATH + "OratorStd.otf");

	//======================== TEXTURES ========================
	textures.push_back(new Texture(TEXTURE_PATH + "Tank1DF.png"));
	textures.push_back(new Texture(TEXTURE_PATH + "grass.png"));
	textures.push_back(new Texture(TEXTURE_PATH + "ground.jpg"));
	textures.push_back(new Texture(TEXTURE_PATH + "rock.jpg"));

	//========================  MODELS  ========================
	models.push_back(new Model(MODEL_PATH + "utah-teapot.FBX"));
	Model *terrainModel = new Model();
	terrainModel->SetUpAttrib(0, 3, GL_FLOAT, 0);
	terrainModel->SetUpAttrib(1, 2, GL_FLOAT, sizeof(vec3) + sizeof(vec4));
	models.push_back(terrainModel);

	//========================  SHADERS ========================
	ShaderProgram *s = new ShaderProgram(SHADER_PATH + "specularVS.glsl", SHADER_PATH + "specularFS.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->BindAttribLoc(3, "vertexNormal");
	s->Link();
	shaders.push_back(s);

	s = new ShaderProgram(SHADER_PATH + "postProcVS.glsl", SHADER_PATH + "colorCorr1.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->Link();
	shaders.push_back(s);

	s = new ShaderProgram(SHADER_PATH + "postProcVS.glsl", SHADER_PATH + "colorCorr2.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->Link();
	shaders.push_back(s);

	s = new ShaderProgram(SHADER_PATH + "simpleVS.glsl", SHADER_PATH + "simpleFS.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->BindAttribLoc(1, "uvs");
	s->Link();
	shaders.push_back(s);

	s = new ShaderProgram(SHADER_PATH + "terrainVS.glsl", SHADER_PATH + "terrainFS.glsl");
	s->BindAttribLoc(0, "vertexPosition");
	s->BindAttribLoc(1, "uvs");
	s->Link();
	shaders.push_back(s);

	//======================== GAMEOBJECTS  ========================
	GameObject *cameraGameObject = new GameObject();
	cameraGameObject->SetName("CameraBehaviourObject");
	camera = new Camera();
	cameraGameObject->AttachComponent(new CameraBehaviour(camera));
	gameObjects.push_back(cameraGameObject);

	GameObject *terrain = new GameObject();
	terrain->SetName("Terrain");
	Renderer *r = new Renderer(textures[1], shaders[4], terrainModel, GL_TRIANGLES);
	r->AddTexture(textures[2]);
	r->AddTexture(textures[3]);
	terrain->AttachComponent(r);
	terrain->AttachComponent(new TerrainComp(TEXTURE_PATH + "heightmap.png", vec3(20, 20, 4)));
	gameObjects.push_back(terrain);

	//testing 10 teapots
	for (int i = 0; i < 0; i++)
	{
		GameObject *go = new GameObject();
		go->SetRotation(vec3(0, i * 36, 0));
		go->SetScale(vec3(0.1f, 0.1f, 0.1f));
		go->SetPos(vec3(i * 5, 0, 0));

		Renderer *renderer = new Renderer();
		renderer->AddTexture(textures[0]);
		renderer->SetModel(models[0], GL_TRIANGLES);
		renderer->SetShaderProgram(shaders[0]);

		go->AttachComponent(renderer);
		gameObjects.push_back(go);
	}
}

void Game::ReleaseResources()
{
	PostProcessing::CleanUp();
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
		//(*iter)->AddRotation(vec3(0, deltaTime * 15, 0));
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
	GLuint aVao = aRenderer->GetModel()->Get();
	GLuint bVao = bRenderer->GetModel()->Get();

	if (aVao == bVao)
	{
		/* Not sorting by texture because we have 5 of textures assigned to renderer potentially
		//since we have same VAOs, sort by texture
		GLuint aText = aRenderer->GetTexture()->Get();
		GLuint bText = bRenderer->GetTexture()->Get();

		if (aText == bText) //since same texture, sort by shader
			return aRenderer->GetProgram() > bRenderer->GetProgram();
		else
			return aText > bText;*/

		return aRenderer->GetProgram() > bRenderer->GetProgram();
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
	glBindFramebuffer(GL_FRAMEBUFFER, PostProcessing::Get());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
		(*iter)->Render(camera);

	//PostProcessing::Pass(shaders[1]);
	//PostProcessing::Pass(shaders[2]); //if you apply shader[1] again you should see the initial image
	PostProcessing::RenderResult();

	font->Flush();
}