#pragma once
#include "MObject.h"

class MSkinnedMesh : public MObject
{
public:
	MSkinnedMesh();
	MSkinnedMesh(void* buffer, unsigned int& tail);
	~MSkinnedMesh();

	void ReadData(void* buffer, unsigned int& tail, PackageType packageType = PACKAGETYPEAMOUNT);
	unsigned int GetVertexCount();
	vector<MeshSkinnedData>* GetData();

private:
	unsigned int _vertexCount;
	vector<MeshSkinnedData> _data;
	SkinnedMeshHeader _header;

	void ReadWhole(void* buffer, unsigned int& tail);
};