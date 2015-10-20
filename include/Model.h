#ifndef _MODEL_H
#define _MODEL_H

#include "Common.h"
#include "Vertex.h"
#include <fbxsdk.h>

class Model
{
private:
	GLuint vao, vbo, ebo;
	vector<Vertex> vertices;
	vector<int> indices;

	bool loadFBXFromFile(const string& fileName);
	void processNode(FbxNode *node, int level);
	void processAttrib(FbxNodeAttribute *attrib, int level);
	void processMesh(FbxMesh *mesh, int level);
	void processMeshTextCoords(FbxMesh *mesh, Vertex *verts, int numVerts);
public:
	Model();
	Model(const string& fileName);
	~Model();

	GLuint Get() { return vao; }
	int GetVertCount() { return vertices.size(); }
	int GetIndCount() { return indices.size(); }
};

#endif