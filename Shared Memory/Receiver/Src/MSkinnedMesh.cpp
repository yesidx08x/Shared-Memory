#include "../Receiver/Inc/MSkinnedMesh.h"

MSkinnedMesh::MSkinnedMesh() {}

unsigned int MSkinnedMesh::GetVertexCount() { return _vertexCount; }

vector<MeshSkinnedData>* MSkinnedMesh::GetData() { return &_data; }

MSkinnedMesh::MSkinnedMesh(void* buffer, unsigned int& tail)
{
	ReadWhole(buffer, tail);
}

MSkinnedMesh::~MSkinnedMesh() {}

void MSkinnedMesh::ReadData(void* buffer, unsigned int& tail, PackageType packageType)
{
	ReadWhole(buffer, tail);
}

void MSkinnedMesh::ReadWhole(void* buffer, unsigned int& tail)
{
	// Header
	memcpy(&_header, (char*)buffer + tail, sizeof(SkinnedMeshHeader));
	tail += sizeof(SkinnedMeshHeader);
	_transformId = _header.transformID;
	_materialId = _header.materialID;
	_vertexCount = _header.vertexCount;
	_skinClusterId = _header.skinClusterID;
	_data.resize(_vertexCount);

	// Data
	memcpy(_data.data(), (char*)buffer + tail, sizeof(MeshSkinnedData) * _vertexCount);
	tail += sizeof(MeshSkinnedData) * _vertexCount;
}