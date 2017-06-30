#include "Mesh.h"

Mesh::Mesh(){}

Mesh::~Mesh(){}

void Mesh::SetData(MObject& object, PackageType packageType)
{
	_identifierSize = 0;
	_size = 0;
	_packageType = packageType;
	MFnMesh mesh(object);
	SetIdentifier(mesh.name());

	switch (packageType)
	{
	case WHOLE:
		_size = GetWhole(mesh);
		break;
	case CREATE:
		_size = GetWhole(mesh);
		break;
	case TRANSFORMID:
		SetTransformID(mesh.parent(0)); // Must see if 0 always is a transform
		_size = sizeof(unsigned int);
		break;
	case MATERIALID:
		_size = GetMaterialID(mesh);
		break;
	case DESTROY:
		SetIdentifier(mesh.name());
		break;
	default:
		break;
	}
}

void Mesh::SendPackage(void* buffer, unsigned int head)
{
	switch (_packageType)
	{
	case WHOLE:
		SendWhole(buffer, head);
		break;
	case CREATE:
		SendWhole(buffer, head);
		break;
	case TRANSFORMID:
		SendTransformID(buffer, head);
		break;
	case MATERIALID:
		SendMaterialID(buffer, head);
		break;
	case DESTROY:
		SendIdentifier(buffer, head);
		break;
	default:
		break;
	}
}

unsigned int Mesh::GetWhole(MFnMesh& mesh)
{
	unsigned int size = 0;
	_header.transformID = SetTransformID(mesh.parent(0));
	_header.transformIDSize = SetTransformIdentifier(mesh.parent(0));
	size += GetTextureNames(mesh);
	_header.materialID = _materialId;

	float2 uv;
	MVector normal;
	MVector tangent;
	MPointArray points;
	MIntArray vertexList;

	_vertexData.clear();

	// Normals are per face
	MItMeshPolygon itPoly(mesh.object());
	while (!itPoly.isDone())
	{
		itPoly.getTriangles(points, vertexList);
		for (int i = (points.length() - 1); i >= 0; i--)
		{
			itPoly.getUVAtPoint(points[i], uv);
			//itPoly.getNormal(normal);
			mesh.getVertexNormal(vertexList[i], true, normal);
			mesh.getFaceVertexNormal(itPoly.index(), vertexList[i], normal);
			mesh.getFaceVertexTangent(itPoly.index(), vertexList[i], tangent);

			_vertexData.push_back(VertexData());
			_vertexData.back().vertex = XMFLOAT3(points[i].x, points[i].y, -points[i].z);
			_vertexData.back().uv = XMFLOAT2(uv[0], 1.0f - uv[1]);
			_vertexData.back().normal = XMFLOAT3(normal.x, normal.y, -normal.z);
			_vertexData.back().tanget = XMFLOAT3(tangent.x, tangent.y, -tangent.z);
		}
		itPoly.next();
	}
	_header.vertexCount = _vertexData.size();
	size += _vertexData.size() * sizeof(VertexData);
	size += sizeof(MeshHeader);
	size += _header.transformIDSize;

	if (_vertexData.size() <= 0)
	{
		_packageType = NONE;
		size = 0;
	}
	return size;
}

unsigned int Mesh::GetMaterialID(MFnMesh& mesh)
{
	// Only supports one shader per mesh
	MObjectArray shaders;
	MIntArray faceArray;
	mesh.getConnectedShaders(0, shaders, faceArray);
	if (shaders.length() > 0)
	{
		MFnDependencyNode depNode(shaders[0]);
		MPlug plug = depNode.findPlug("surfaceShader");
		MPlugArray materialArray;
		plug.connectedTo(materialArray, true, false);
		SetMaterialID(materialArray[0].node());
	}
	else
	{
		_materialId = 0;
	}
	return sizeof(unsigned int);
}

unsigned int Mesh::GetTextureNames(MFnMesh& mesh)
{
	unsigned int size = 0;
	MObjectArray shaders;
	MIntArray faceArray;
	mesh.getConnectedShaders(0, shaders, faceArray);
	_header.materialIDSize = 0;
	_textureNames.clear();
	if (shaders.length() > 0)
	{
		MFnDependencyNode shaderdepNode(shaders[0]);
		MPlug surfacePlug = shaderdepNode.findPlug("surfaceShader");
		MPlugArray materialArray;
		surfacePlug.connectedTo(materialArray, true, false);
		MFnDependencyNode materialDepNode(materialArray[0].node());

		// Look for texture path if there is one
		MPlugArray plugs;
		materialDepNode.getConnections(plugs);
		for (size_t i = 0; i < plugs.length(); i++)
		{
			//Print(plugs[i].name());
		}

		MPlug albedoPlug = materialDepNode.findPlug("TEX_color_map", true, &_status);
		size += AddTexture(albedo, albedoPlug);
		MPlug roughnessPlug = materialDepNode.findPlug("TEX_roughness_map", true, &_status);
		size += AddTexture(roughness, roughnessPlug);
		MPlug metallicPlug = materialDepNode.findPlug("TEX_metallic_map", true, &_status);
		size += AddTexture(metallic, metallicPlug);
		MPlug normalPlug = materialDepNode.findPlug("TEX_normal_map", true, &_status);
		size += AddTexture(normal, normalPlug);
	}
	return size;
}

unsigned int Mesh::AddTexture(MaterialType type, MPlug& plug)
{
	unsigned int size = 0;
	if (_status == MS::kSuccess)
	{
		MItDependencyGraph itGraph(plug, MFn::kFileTexture, MItDependencyGraph::kUpstream);
		MString texturePath;
		if (!itGraph.isDone())
		{
			_textureNames.push_back(MaterialData());
			MFnDependencyNode(itGraph.thisNode()).findPlug("fileTextureName", &_status).getValue(texturePath);
			_textureNames.back().type = type;
			_textureNames.back().identifier = texturePath.asChar();
			_header.materialIDSize++;
			size = _textureNames.back().identifier.size() + sizeof(unsigned int) * 2;
		}
	}
	return size;
}

void Mesh::SendWhole(void* buffer, unsigned int head)
{
	// Identifier
	memcpy((char*)buffer + head, _identifier.data(), _identifierSize);
	head += _identifierSize;
	// Header
	memcpy((char*)buffer + head, &_header, sizeof(MeshHeader));
	head += sizeof(MeshHeader);
	// Transform ID
	memcpy((char*)buffer + head, _transformIdentifier.data(), _header.transformIDSize);
	head += _header.transformIDSize;
	// Materials
	for (size_t i = 0; i < _header.materialIDSize; i++)
	{
		memcpy((char*)buffer + head, &_textureNames[i].type, sizeof(unsigned int));
		head += sizeof(unsigned int);
		unsigned int size = _textureNames[i].identifier.size();
		memcpy((char*)buffer + head, &size, sizeof(unsigned int));
		head += sizeof(unsigned int);
		memcpy((char*)buffer + head, _textureNames[i].identifier.data(), size);
		head += size;
	}
	// Data
	memcpy((char*)buffer + head, _vertexData.data(), _vertexData.size() * sizeof(VertexData));
	head += _vertexData.size() * sizeof(VertexData);
}