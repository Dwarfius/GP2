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

GLuint indcs[] = {
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
	int count = sizeof(verts) / sizeof(Vertex);
	for (int i = 0; i < count; i++)
		vertices.push_back(verts[i]);

	count = sizeof(indcs) / sizeof(GLuint);
	for (int i = 0; i < count; i++)
		indices.push_back(indcs[i]);

	//generate and bind vao so that it keeps the current vbo and ebo and attribs
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	CHECK_GL_ERROR();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * GetVertCount(), vertices.data(), GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * GetIndCount(), indices.data(), GL_STATIC_DRAW);
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

Model::Model(const string& fileName)
{
	if (!loadFBXFromFile(fileName))
	{
		printf("Error loading model!\n");
		return;
	}
	printf("Verts: %d Ints: %d\n", vertices.size(), indices.size());

	//generate and bind vao so that it keeps the current vbo and ebo and attribs
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	CHECK_GL_ERROR();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * GetVertCount(), vertices.data(), GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * GetIndCount(), indices.data(), GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	//Tell the shader that 0 is the position element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	CHECK_GL_ERROR();

	//tell the shader that 1 is the color element
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)));
	CHECK_GL_ERROR();

	//uv
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3) + sizeof(vec4)));
	CHECK_GL_ERROR();

	//normals
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3) + sizeof(vec4) + sizeof(vec2)));
	CHECK_GL_ERROR();
}

Model::~Model()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vao);

	vertices.clear();
	indices.clear();
}

//utility function
void PrintTabs(int level)
{
	for (int i = 0; i < level; i++)
		printf("\t");
}

//utility function
FbxString GetAttribTypeName(FbxNodeAttribute::EType type)
{
	switch (type)
	{
	case FbxNodeAttribute::eUnknown: return "unknown";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}

bool Model::loadFBXFromFile(const string& fileName)
{
	//initialize the sdk manager
	FbxManager *manager = FbxManager::Create();

	//setting up the IO settings object
	FbxIOSettings *settings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(settings);

	//creating the importer
	FbxImporter *importer = FbxImporter::Create(manager, "");

	//initializing the importer with contents of file
	if (!importer->Initialize(fileName.c_str(), -1, settings))
		return false;

	//creating a scene to be filled with objects
	FbxScene *scene = FbxScene::Create(manager, "myScene");
	importer->Import(scene);

	FbxGeometryConverter geomConverter(manager);
	geomConverter.Triangulate(scene, true);

	FbxNode *root = scene->GetRootNode();
	if (root)
	{
		printf("Root Node: %s\n", root->GetName());
		int childCount = root->GetChildCount();
		for (int i = 0; i < childCount; i++)
			processNode(root->GetChild(i), 1);
	}
	importer->Destroy();
	return true;
}

void Model::processNode(FbxNode *node, int level)
{
	PrintTabs(level);
	const char* name = node->GetName();
	FbxDouble3 translation = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	FbxDouble3 scaling = node->LclScaling.Get();

	printf("Node %s(%.1f, %.1f, %.1f)/(%.1f, %.1f, %.1f)/(%.1f, %.1f, %.1f)\n", 
		name, translation[0], translation[1], translation[2],
		rotation[0], rotation[1], rotation[2], scaling[0], scaling[1],
		scaling[2]);

	int count = node->GetNodeAttributeCount();
	for (int i = 0; i < count; i++)
		processAttrib(node->GetNodeAttributeByIndex(i), level + 1);

	count = node->GetChildCount();
	for (int i = 0; i < count; i++)
		processNode(node->GetChild(i), level + 1);

	PrintTabs(level);
}

void Model::processAttrib(FbxNodeAttribute *attrib, int level)
{
	if (!attrib) return;

	FbxString typeName = GetAttribTypeName(attrib->GetAttributeType());
	FbxString attrName = attrib->GetName();
	PrintTabs(level);
	printf("Attribute %s Name %s\n", typeName.Buffer(), attrName);
	switch (attrib->GetAttributeType())
	{
	case FbxNodeAttribute::eMesh: 
		processMesh(attrib->GetNode()->GetMesh(), level); 
		break;
	case FbxNodeAttribute::eCamera:
		return;
	case FbxNodeAttribute::eLight:
		return;
	}
}

void Model::processMesh(FbxMesh *mesh, int level)
{
	int numVerts = mesh->GetControlPointsCount();
	int numInds = mesh->GetPolygonVertexCount();

	Vertex *pVerts = new Vertex[numVerts];
	int *pIndices = mesh->GetPolygonVertices();

	for (int i = 0; i < numVerts; i++)
	{
		FbxVector4 vert = mesh->GetControlPointAt(i);
		pVerts[i].pos = vec3(vert[0], vert[1], vert[2]);
		pVerts[i].color = vec4(1, 1, 1, 1);
		pVerts[i].texture = vec2(0, 0);
	}

	processMeshTextCoords(mesh, pVerts, numVerts);
	processMeshNormals(mesh, pVerts, numVerts);
	uint initVertCount = vertices.size();
	for (int i = 0; i < numVerts; i++)
		vertices.push_back(pVerts[i]);

	for (int i = 0; i < numInds; i++)
		indices.push_back(initVertCount + pIndices[i]);

	PrintTabs(level);
	printf("Vertices %d Indices %d\n", numVerts, numInds);

	delete[] pVerts;
}

void Model::processMeshTextCoords(FbxMesh *mesh, Vertex *verts, int numVerts)
{
	int polCount = mesh->GetPolygonCount();
	for (int polInd = 0; polInd < polCount; polInd++)
	{
		for (unsigned polVert = 0; polVert < 3; polVert++)
		{
			int cornerIndex = mesh->GetPolygonVertex(polInd, polVert);
			FbxVector2 UV = FbxVector2(0, 0);
			FbxLayerElementUV *layerUV = mesh->GetLayer(0)->GetUVs();
			if (layerUV)
			{
				int UVindex = 0;
				switch (layerUV->GetMappingMode())
				{
				case FbxLayerElement::eByControlPoint:
					UVindex = cornerIndex;
					break;
				case FbxLayerElement::eByPolygonVertex:
					UVindex = mesh->GetTextureUVIndex(polInd, polVert, FbxLayerElement::eTextureDiffuse);
					break;
				case FbxLayerElement::eByPolygon:
					UVindex = polInd;
					break;
				}

				UV = layerUV->GetDirectArray().GetAt(UVindex);
				verts[cornerIndex].texture.x = UV[0];
				verts[cornerIndex].texture.y = 1.f - UV[1];
			}
		}
	}
}

void Model::processMeshNormals(FbxMesh *mesh, Vertex *verts, int count)
{
	int polCount = mesh->GetPolygonCount();
	for (int polInd = 0; polInd < polCount; polInd++)
	{
		for (int vertInd = 0; vertInd < 3; vertInd++)
		{
			int cornerIndex = mesh->GetPolygonVertex(polInd, vertInd);
			FbxVector4 normal;
			mesh->GetPolygonVertexNormal(polInd, vertInd, normal);
			normal.Normalize();
			verts[cornerIndex].normal.x = normal[0];
			verts[cornerIndex].normal.y = normal[1];
			verts[cornerIndex].normal.z = normal[2];
		}
	}
}