#pragma once
#include "MObject.h"

class MMesh : public MObject
{
public:
	MMesh();
	MMesh(void* buffer, unsigned int& tail);
	~MMesh();

	void ReadData(void* buffer, unsigned int& tail, PackageType packageType = PACKAGETYPEAMOUNT);
	unsigned int GetVertexCount();
	MeshData* GetData();
	vector<MaterialData> GetMaterialData();

private:
	unsigned int _oldVertexCount;
	bool _changedSize;
	MeshData* _data;
	vector<MaterialData> _materialData;
	MeshHeader _header;

	void ReadWhole(void* buffer, unsigned int& tail);
};