#pragma once
#include "Object.h"

class Mesh : public Object
{
public:
	Mesh();
	~Mesh();

	void SetData(MObject& object, PackageType packageType);
	void SendPackage(void* buffer, unsigned int head);

private:
	MeshHeader _header;
	MeshData _data;
	vector<VertexData> _vertexData;
	vector<MaterialData> _textureNames;

	unsigned int GetWhole(MFnMesh& mesh);
	unsigned int GetMaterialID(MFnMesh& mesh);
	unsigned int GetTextureNames(MFnMesh& mesh);
	unsigned int AddTexture(MaterialType type, MPlug& plug);
	void SendWhole(void* buffer, unsigned int head);
};