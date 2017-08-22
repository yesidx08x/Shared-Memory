#pragma once
#include "MMesh.h"
#include "MCamera.h"
#include "MTransform.h"
#include "MLight.h"
#include "MMaterial.h"
#include "SharedMemory.h"

class MDataHandler
{
public:
	MDataHandler();
	~MDataHandler();

	DataType Update();
	bool SharedOpen();
	bool DataToRead();
	void ShutDown();
	MessageHeader GetMessageHeader();

	MMesh* GetMeshe(string identifier);
	MTransform* GetTransform(string identifier);
	MLight* GetLight(string identifier);
	MMaterial* GetMaterial(string identifier);
	string GetIdentifier();
	MCamera* GetCamera();
	UINT32* GetMeshStrides();
	UINT32* GetMeshOffsets();

private:
	SharedMemory _sharedMemory;
	SharedMemory::CircInfo _circInfo;
	PackageType _packageType;

	MessageHeader _messageHeader;

	UINT32 meshStrides = 12 + 8 + 12 + 12;
	UINT32 meshOffsets = 0;

	MCamera _camera;
	map<string, MMesh*> _meshes;
	map<string, MLight*> _lights;
	map<string, MMaterial*> _materials;
	map<string, MTransform*> _transforms;
	string _identifier;
	void MeshHandler();
	void TransformHandler();
	void LightHandler();
	void MaterialHandler();
	void CameraHandler();
};