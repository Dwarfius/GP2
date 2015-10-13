#include "Model.h"
#include "Vertex.h"

Vertex verts[] = {
	//front
	{ vec3(-0.5f, 0.5f, 0.5f), vec4(1, 1, 1, 1), vec2(0, 0) },	//top-left
	{ vec3(-0.5f, -0.5f, 0.5f), vec4(1, 1, 1, 1), vec2(0, 1) }, //bottom-left
	{ vec3(0.5f, -0.5f, 0.5f), vec4(1, 1, 1, 1), vec2(1, 1) }, //bottom-right
	{ vec3(0.5f, 0.5f, 0.5f), vec4(1, 1, 1, 1), vec2(1, 0) }, //top-right

												  //back
	{ vec3(-0.5f, 0.5f, -0.5f), vec4(1, 1, 1, 1), vec2(0, 0) },	//top-left
	{ vec3(-0.5f, -0.5f, -0.5f), vec4(1, 1, 1, 1), vec2(0, 1) }, //bottom-left
	{ vec3(0.5f, -0.5f, -0.5f), vec4(1, 1, 1, 1), vec2(1, 1) }, //bottom-right
	{ vec3(0.5f, 0.5f, -0.5f), vec4(1, 1, 1, 1), vec2(1, 0) } //top-right
};

GLuint indices[] = {
	0, 1, 2, //front
	0, 3, 2,

	4, 5, 1, //left
	4, 1, 0,

	3, 7, 2, //right
	7, 6, 2,

	0, 4, 7, //top
	0, 3, 7,

	1, 5, 6, //bottom
	1, 2, 6,

	4, 5, 6, //back
	4, 7, 6
};

Model::Model()
{
	//generate and bind vao so that it keeps the current vbo and ebo and attribs
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	CHECK_GL_ERROR();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	//Tell the shader that 0 is the position element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	CHECK_GL_ERROR();

	//tell the shader that 1 is the color element
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)));
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3) + sizeof(vec4)));
	CHECK_GL_ERROR();
}

Model::~Model()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vao);
}

GLsizei Model::Count()
{
	return sizeof(indices) / sizeof(GLuint);
}
