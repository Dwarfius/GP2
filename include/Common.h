#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>

//maths
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

const string ASSET_PATH = "assets/";
const string SHADER_PATH = ASSET_PATH + "shaders/";
const string TEXTURE_PATH = ASSET_PATH + "textures/";
const string FONT_PATH = ASSET_PATH + "fonts/";
const string MODEL_PATH = ASSET_PATH + "models/";

void checkError(const char *file, int line);
#define CHECK_GL_ERROR() checkError(__FILE__, __LINE__)

void printVec3(vec3 p);
void printRect(SDL_Rect r);

#endif