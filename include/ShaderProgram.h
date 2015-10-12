#ifndef _SHADERPROG_H
#define _SHADERPROG_H

#include "Common.h"
#include "Shader.h"

class ShaderProgram
{
private:
	Shader *vShader, *fShader;
	GLuint program;

	bool CheckForLinkErrors();

public:
	ShaderProgram(const string& vShaderFile, const string& fShaderFile);
	~ShaderProgram();

	GLuint Get() { return program; }
};

#endif