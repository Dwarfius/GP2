#include "Common.h"
#include "Graphics.h"
#include "Input.h"
#include "Game.h"

vector<string> split(const string& s, const char delim) 
{
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

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

void printRect(SDL_Rect r)
{
	printf("(%f, %f, %f, %f)\n", r.x, r.y, r.w, r.h);
}

bool run = true;
bool paused = false;

int main(int argc, char *argv[])
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
							SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H,
							SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	CHECK_GL_ERROR();
	Graphics::Init();
	Graphics::SetViewport(SCREEN_W, SCREEN_H);
	CHECK_GL_ERROR();

	if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0)
		printf("Failed to set mouse mode to Relative\n");

	Game *game = new Game();
	game->LoadResources();
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

		if (Input::GetKey(SDLK_ESCAPE))
			run = false;
		if (Input::GetKeyDown(SDLK_SPACE))
			paused = !paused;
		if (paused)
			continue;

		game->Update(deltaTime);
		CHECK_GL_ERROR();
		game->Render(deltaTime);
		CHECK_GL_ERROR();

		SDL_GL_SwapWindow(window);
		CHECK_GL_ERROR();
	}

	game->ReleaseResources();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

    return 0;
}
