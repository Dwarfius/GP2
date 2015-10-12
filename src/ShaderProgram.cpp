#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const string& vShaderFile, const string& fShaderFile)
{
	vShader = new Shader(vShaderFile, VERTEX_SHADER);
	fShader = new Shader(fShaderFile, FRAGMENT_SHADER);
	CHECK_GL_ERROR();

	program = glCreateProgram();
	glAttachShader(program, vShader->Get());
	glAttachShader(program, fShader->Get());
	glLinkProgram(program);
	CheckForLinkErrors();
	CHECK_GL_ERROR();

	//if they were linked we no longer need them
	delete vShader;
	delete fShader;

	//standard attribs
	glBindAttribLocation(program, 0, "vertexPos");
	CHECK_GL_ERROR();
	glBindAttribLocation(program, 1, "color");
	CHECK_GL_ERROR();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program);
}

bool ShaderProgram::CheckForLinkErrors()
{
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

	if (isLinked == GL_FALSE)
	{
		GLint length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		char *msg = (char*)malloc(length);
		glGetProgramInfoLog(program, length, &length, msg);

		printf("Shader linking error: %s\n", msg);
		free(msg);

		glDeleteProgram(program);
		return true;
	}
	return false;
}