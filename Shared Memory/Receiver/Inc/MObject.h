#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "DataStructures.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class MObject
{
public:
	MObject();
	~MObject();

	virtual void ReadData(void* buffer, unsigned int& tail, PackageType packageType = PACKAGETYPEAMOUNT) = 0;
	ID3D11Buffer** GetBuffer();
	void ReleaseBuffer();
	void SetBuffer(ID3D11Buffer* buffer);
	unsigned int GetTransformId();
	string GetTransformID();
	unsigned int GetMaterialId();
	unsigned int GetTextureId();
	unsigned int GetSkinClusterId();

	void ReadTransformID(void* buffer, unsigned int& tail);
	void ReadMaterialID(void* buffer, unsigned int& tail);

protected:

	unsigned int _size;
	unsigned int _id;
	PackageType _packageType;
	int _transformId;
	string _transformIdentifier;
	int _materialId;
	int _textureId;
	int _skinClusterId;

	ID3D11Buffer* _bufferData;
};