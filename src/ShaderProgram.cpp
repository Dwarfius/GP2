#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const string& vShaderFile, const string& fShaderFile)
{
	vShader = new Shader(vShaderFile, VERTEX_SHADER);
	fShader = new Shader(fShaderFile, FRAGMENT_SHADER);
	CHECK_GL_ERROR();

	program = glCreateProgram();
	glAttachShader(program, vShader->Get());
	glAttachShader(program, fShader->Get());
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program);
}

void ShaderProgram::BindAttribLoc(GLuint loc, const char *name)
{
	glBindAttribLocation(program, loc, name);
	CHECK_GL_ERROR();
}

void ShaderProgram::GetUniformLoc(const char *name)
{

}

void ShaderProgram::Link()
{
	//link shaders and attribs
	glLinkProgram(program);
	CheckForLinkErrors();
	CHECK_GL_ERROR();

	//if they were linked we no longer need them
	delete vShader;
	delete fShader;
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