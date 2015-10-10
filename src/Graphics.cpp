#include "Graphics.h"
#include "Vertices.h"
#include "Common.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::Init()
{
	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK)
		cout << "GLEW Error: " << glewGetErrorString(err) << endl;

	//ignoring the first error
	glGetError();

	//Smooth shading
	glShadeModel(GL_SMOOTH);

	//clear with black
	glClearColor(0, 0, 0, 0);

	//clear depth to 1
	glClearDepth(1);

	//enable depth testing
	glEnable(GL_DEPTH_TEST);

	//set less or equal func for depth testing
	glDepthFunc(GL_LEQUAL);

	//turn on best perspective correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	CHECK_GL_ERROR();
}

void Graphics::SetViewport(int width, int height)
{
	if (height == 0)
		height = 1;
	
	//setting up the viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}