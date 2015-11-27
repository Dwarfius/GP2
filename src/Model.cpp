#include "Model.h"
#include "Vertex.h"

Model::Model()
{
	vertices = new vector<Vertex>();
	indices = new vector<int>();

	//generate and bind vao so that it keeps the current vbo and ebo and attribs
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	CHECK_GL_ERROR();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	CHECK_GL_ERROR();

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	CHECK_GL_ERROR();
}

Model::Model(const string& fileName)
{
	vertices = new vector<Vertex>();
	indices = new vector<int>();

	if (!loadFBXFromFile(fileName))
	{
		printf("Error loading model!\n");
		return;
	}
	printf("Verts: %d Ints: %d\n", vertices->size(), indices->size());

	//generate and bind vao so that it keeps the current vbo and ebo and attribs
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	CHECK_GL_ERROR();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * GetVertCount(), vertices->data(), GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * GetIndCount(), indices->data(), GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	//tell the VAO that 1 is the position element
	SetUpAttrib(0, 3, GL_FLOAT, 0);

	//tell the VAO that 1 is the color element
	SetUpAttrib(1, 4, GL_FLOAT, sizeof(vec3));

	//uv
	SetUpAttrib(2, 2, GL_FLOAT, sizeof(vec3) + sizeof(vec4));

	//normals
	SetUpAttrib(3, 3, GL_FLOAT, sizeof(vec3) + sizeof(vec4) + sizeof(vec2));

	//tangent
	SetUpAttrib(4, 3, GL_FLOAT, sizeof(vec3) + sizeof(vec4) + sizeof(vec2) + sizeof(vec3));

	//binormal
	SetUpAttrib(5, 3, GL_FLOAT, sizeof(vec3) + sizeof(vec4) + sizeof(vec2) + sizeof(vec3) + sizeof(vec3));
}

void Model::SetVertices(vector<Vertex> *verts, GLuint flag, bool deletePrev)
{
	GLint activeBuffer;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &activeBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if(deletePrev)
		delete vertices;
	vertices = verts;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * GetVertCount(), vertices->data(), flag);
	glBindBuffer(GL_ARRAY_BUFFER, activeBuffer);
	CHECK_GL_ERROR();
}

void Model::SetIndices(vector<int> *indcs, GLuint flag, bool deletePrev)
{
	GLint activeBuffer; 
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &activeBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	if(deletePrev)
		delete indices;
	indices = indcs;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * GetIndCount(), indices->data(), flag);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, activeBuffer);
	CHECK_GL_ERROR();
}

void Model::SetUpAttrib(int index, int count, int type, size_t offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, count, type, GL_FALSE, sizeof(Vertex), (void**)offset);
	CHECK_GL_ERROR();
}

void Model::SetAttribDivisor(int index, int divisor)
{
	glVertexAttribDivisor(index, divisor);
}

Model::~Model()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vao);

	delete vertices;
	delete indices;
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
		//pVerts[i].tangent = processMeshTangent(mesh, i);
		//pVerts[i].binormal = processMeshBinormal(mesh, i);
	}

	processMeshTextCoords(mesh, pVerts, numVerts);
	processMeshNormals(mesh, pVerts, numVerts);
	
	uint initVertCount = vertices->size();
	for (int i = 0; i < numVerts; i++)
		vertices->push_back(pVerts[i]);

	for (int i = 0; i < numInds; i++)
		indices->push_back(initVertCount + pIndices[i]);

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
			verts[cornerIndex].tangent = getTangent(verts[cornerIndex].normal);
			verts[cornerIndex].binormal = getBinormal(verts[cornerIndex].normal, verts[cornerIndex].tangent);
		}
	}
}

vec3 Model::getTangent(vec3 normal)
{
	vec3 tangent;
	vec3 c1 = cross(normal, vec3(0.0, 0.0, 1.0));
	vec3 c2 = cross(normal, vec3(0.0, 1.0, 0.0));

	if (length(c1)>length(c2))
	{
		tangent = c1;
	}
	else
	{
		tangent = c2;
	}

	tangent = normalize(tangent);
	return tangent;
}

vec3 Model::getBinormal(vec3 normal, vec3 tangent)
{
	vec3 binormal = cross(tangent, normal);
	binormal = normalize(binormal);
	return binormal;
}


/*vec3 Model::processMeshTangent(FbxMesh * mesh, int vertsIndex)
{
	if (mesh->GetElementTangentCount() < 1)
	{
		return vec3();
	}

	vec3 outTangent;
	FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);

	outTangent.x = vertexTangent->GetDirectArray().GetAt(vertsIndex).mData[0];
	outTangent.y = vertexTangent->GetDirectArray().GetAt(vertsIndex).mData[1];
	outTangent.z = vertexTangent->GetDirectArray().GetAt(vertsIndex).mData[2];
	return outTangent;
}

vec3 Model::processMeshBinormal(FbxMesh * mesh, int vertsIndex)
{
	if (mesh->GetElementBinormalCount() < 1)
	{
		return vec3();
	}
	vec3 outBinormal;
	FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);

	outBinormal.x = vertexBinormal->GetDirectArray().GetAt(vertsIndex).mData[0];
	outBinormal.y = vertexBinormal->GetDirectArray().GetAt(vertsIndex).mData[1];
	outBinormal.z = vertexBinormal->GetDirectArray().GetAt(vertsIndex).mData[2];
	return outBinormal;
}*/
