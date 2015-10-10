#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

//maths
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#elif (_WIN64 || _WIN32)
#include <gl\GLU.h>
#endif

using namespace std;
using namespace glm;

const string ASSET_PATH = "assets";
const string SHADER_PATH = "/shaders";

void checkError(const char *file, int line);
#define CHECK_GL_ERROR() checkError(__FILE__, __LINE__)

void printVec3(vec3 p);


#endif
