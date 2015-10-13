#include "Common.h"
#include "Graphics.h"
#include "Input.h"
#include "Camera.h"
#include "GameObject.h"

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

bool run = true;
bool paused = false;
vec3 color;

Camera camera;
GameObject *obj;

void InitScene()
{
	obj = new GameObject();
}

void CleanUp()
{
	delete obj;
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

	//obj->AddRotation(vec3(5, 5, 0) * deltaTime);

	/*vec3 red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1);
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
		color = mix(red, green, timer / 5);*/
}

void Render()
{
	camera.Recalculate();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	obj->Render(&camera);
}

int main(int argc, char * arg[])
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "ERROR SDL_Init" << SDL_GetError() << endl;
		return -1;
	}

	int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int returnInitFlags = IMG_Init(imageInitFlags);
	if ((returnInitFlags & imageInitFlags) != imageInitFlags)
	{
		cout << "Error initializing SDL_Image " << IMG_GetError() << endl;
		return -1;
	}

	if (TTF_Init() == -1)
	{
		printf("TTF Init error: %s\n", TTF_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_Window *window = SDL_CreateWindow("GP2", SDL_WINDOWPOS_CENTERED, 
							SDL_WINDOWPOS_CENTERED, 640, 480, 
							SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	CHECK_GL_ERROR();
	Graphics::Init();
	Graphics::SetViewport(640, 480);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	CHECK_GL_ERROR();

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
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

    return 0;
}
