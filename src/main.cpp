#include "Common.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "Shader.h"
#include "Camera.h"

void checkError(const char *file, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		printf("%s:%d - GL Error: %s\n", file, line, gluErrorString(err));
}

void printVec3(vec3 p)
{
	printf("%f/%f/%f\n", p.x, p.y, p.z);
}

GLuint vbo, ebo, vao;
float angle = 0;
bool run = true;
bool paused = false;
vec3 color;

Camera camera;
GLuint shaderProgram = 0;

Vertex verts[] = {
	//front
	{ vec3(-0.5f, 0.5f, 0.5f), vec4(1, 0, 0, 0) },	//top-left
	{ vec3(-0.5f, -0.5f, 0.5f), vec4(0, 1, 0, 1) }, //bottom-left
	{ vec3(0.5f, -0.5f, 0.5f), vec4(0, 0, 1, 1) }, //bottom-right
	{ vec3(0.5f, 0.5f, 0.5f), vec4(0, 1, 0, 1) }, //top-right

	//back
	{ vec3(-0.5f, 0.5f, -0.5f), vec4(0, 1, 0, 1) },	//top-left
	{ vec3(-0.5f, -0.5f, -0.5f), vec4(0, 0, 1, 1) }, //bottom-left
	{ vec3(0.5f, -0.5f, -0.5f), vec4(0, 1, 0, 1) }, //bottom-right
	{ vec3(0.5f, 0.5f, -0.5f), vec4(1, 0, 0, 1) } //top-right
};

int indices[] = {
	0, 1, 2, //front
	0, 3, 2,

	4, 5, 1, //left
	4, 1, 0,

	3, 7, 2, //right
	7, 6, 2,

	0, 4, 7, //top
	0, 3, 7,

	1, 5, 6, //bottom
	1, 2, 6,

	4, 5, 6, //back
	4, 7, 6
};

void InitScene()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	CHECK_GL_ERROR();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	//Tell the shader that 0 is the position element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)));
	CHECK_GL_ERROR();

	string path = ASSET_PATH + SHADER_PATH;
	Shader vShader(path + "/simpleVS.glsl", VERTEX_SHADER);
	Shader fShader(path + "/simpleFS.glsl", FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vShader.Get());
	glAttachShader(shaderProgram, fShader.Get());
	glLinkProgram(shaderProgram);
	Shader::CheckForLinkErrors(shaderProgram);

	glBindAttribLocation(shaderProgram, 0, "vertexPos");
	CHECK_GL_ERROR();
	glBindAttribLocation(shaderProgram, 1, "color");
	CHECK_GL_ERROR();
}

void CleanUp()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vao);

	glDeleteProgram(shaderProgram);
}

float timer = 0;
void Update(float deltaTime)
{
	if (Input::GetKey(SDLK_ESCAPE))
		run = false;

	vec3 forward = camera.GetForward();
	vec3 right = camera.GetRight();
	if (Input::GetKey(SDLK_w))
		camera.Translate(forward * deltaTime);
	if (Input::GetKey(SDLK_s))
		camera.Translate(-forward * deltaTime);
	if (Input::GetKey(SDLK_d))
		camera.Translate(right * deltaTime);
	if (Input::GetKey(SDLK_a))
		camera.Translate(-right * deltaTime);

	/*if (Input::GetMouseBtnDown(SDL_BUTTON_LEFT))
		printf("Left Down!\n");
	if (Input::GetMouseBtn(SDL_BUTTON_LEFT))
		printf("Left held down!\n");
	if (Input::GetMouseBtnUp(SDL_BUTTON_LEFT))
		printf("Left Up!\n");*/

	ivec2 deltaPos = Input::GetMouseDelta();
	camera.Rotate((float)deltaPos.x * deltaTime, (float)-deltaPos.y * deltaTime);

	vec3 red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1);
	timer += deltaTime;
	if (timer > 15)
	{
		color = red;
		timer = 0;
	}
	else if (timer > 10)
		color = mix(blue, red, (timer - 10) / 5);
	else if (timer > 5)
		color = mix(green, blue, (timer - 5) / 5);
	else
		color = mix(red, green, timer / 5);
}

void Render()
{
	camera.Recalculate();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vao);
	glUseProgram(shaderProgram);

	GLint loc = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(camera.Get()));

	loc = glGetUniformLocation(shaderProgram, "colorIn");
	glUniform3f(loc, color.r, color.g, color.b);

	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

int main(int argc, char * arg[])
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "ERROR SDL_Init" << SDL_GetError() << endl;
		return-1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_Window *window = SDL_CreateWindow("Lab1", SDL_WINDOWPOS_CENTERED, 
							SDL_WINDOWPOS_CENTERED, 640, 480, 
							SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	CHECK_GL_ERROR();
	Graphics::Init();
	Graphics::SetViewport(640, 480);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	InitScene();
	CHECK_GL_ERROR();

	Uint32 oldTicks = 0;
	SDL_Event event;
	while (run)
	{
		Input::Update();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				run = false;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				run = false;
				break;
			case SDL_KEYDOWN:
				Input::SetKey(event.key.keysym.sym, true);
				Input::SetMod(event.key.keysym.mod);
				break;
			case SDL_KEYUP:
				Input::SetKey(event.key.keysym.sym, false);
				Input::SetMod(event.key.keysym.mod);
				break;
			case SDL_MOUSEMOTION:
				Input::MoveMouse(ivec2(event.motion.xrel, event.motion.yrel));
				break;
			}
		}

		Uint32 ticks = SDL_GetTicks();
		float deltaTime = (ticks - oldTicks) / 1000.f;
		oldTicks = ticks;

		if (Input::GetKeyDown(SDLK_SPACE))
			paused = !paused;

		if (paused)
			continue;

		Update(deltaTime);
		CHECK_GL_ERROR();
		Render();
		CHECK_GL_ERROR();

		SDL_GL_SwapWindow(window);
	}

	CleanUp();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 0;
}
