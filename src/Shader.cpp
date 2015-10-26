#include "Shader.h"

Shader::Shader(const string& fileName, SHADER_TYPE type)
{
	shader = LoadShaderFromFile(fileName, type);
	if (CheckForCompileErrors(fileName))
		shader = 0;
}

Shader::~Shader()
{
	if (shader != 0)
		glDeleteShader(shader);
}

GLuint Shader::LoadShaderFromMemory(const char *pMem, SHADER_TYPE type)
{
	GLuint shader = glCreateShader(type);
	//printf("Shader:\n%s\n\n", pMem);
	glShaderSource(shader, 1, &pMem, NULL);
	glCompileShader(shader);
	return shader;
}

GLuint Shader::LoadShaderFromFile(const string& fileName, SHADER_TYPE type)
{
	ifstream file;
	file.open(fileName.c_str(), ios::in);
	if (!file)
	{
		printf("Couldn't load file: %s\n", fileName.c_str());
		return 0;
	}

	if (file.good())
	{
		file.seekg(0, ios::end);
		unsigned long len = file.tellg();
		file.seekg(0, ios::beg);
		if (len == 0)
		{
			printf("Empty file: %s\n", fileName.c_str());
			return 0;
		}

		char *contents = (char*)malloc(len);
		memset(contents, 0, len);
		file.read(contents, len);
		//printf("%d-%d\n", len, strlen(contents));
		file.close();
		GLuint shader = LoadShaderFromMemory(contents, type);
		free(contents);
		return shader;
	}
	return 0;
}

bool Shader::CheckForCompileErrors(const string& fileName)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint errorLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLength);

		char *msg = (char*)malloc(errorLength);
		glGetShaderInfoLog(shader, errorLength, &errorLength, msg);

		printf("Error compiling a shader(%s): %s\n", fileName.c_str(), msg);
		free(msg);

		glDeleteShader(shader);
		return true;
	}
	return false;
}