#ifndef _MODEL_H
#define _MODEL_H

#include "Common.h"
#include "Vertex.h"
#include <fbxsdk.h>

class Model
{
private:
	GLuint vao, vbo, ebo;
	vector<Vertex> *vertices;
	vector<int> *indices;

	bool loadFBXFromFile(const string& fileName);
	void processNode(FbxNode *node, int level);
	void processAttrib(FbxNodeAttribute *attrib, int level);
	void processMesh(FbxMesh *mesh, int level);
	void processMeshTextCoords(FbxMesh *mesh, Vertex *verts, int numVerts);
	void processMeshNormals(FbxMesh *mesh, Vertex *verts, int count);
public:
	//creates a generic model - requires attrib bindinding!
	Model();
	//creates a model from file - attribs already bound
	Model(const string& fileName);
	~Model();

	GLuint Get() { return vao; }
	int GetVertCount() { return vertices->size(); }
	int GetIndCount() { return indices->size(); }

	//takes ownership of verts! releases memory on it's own!
	void SetVertices(vector<Vertex> *verts, GLuint flag, bool deletePrev);
	//takes ownership of ints! releases memory on it's own!
	void SetIndices(vector<int> *indcs, GLuint flag, bool deletePrev);

	void SetUpAttrib(int index, int count, int type, size_t offset);
};

#endif