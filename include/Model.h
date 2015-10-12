#ifndef _MODEL_H
#define _MODEL_H

#include "Common.h"

class Model
{
private:
	GLuint vao, vbo, ebo;

public:
	Model();
	~Model();

	GLuint Get() { return vao; }
	GLsizei Count();
};

#endif