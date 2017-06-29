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
	void ShutDown();
	MessageHeader GetMessageHeader();

	vector<MMesh*>* GetMeshes();
	vector<MTransform*>* GetTransforms();
	vector<MLight*>* GetLights();
	vector<MMaterial*>* GetMaterials();
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
	vector<MMesh*> _meshes;
	vector<MLight*> _lights;
	vector<MMaterial*> _materials;
	vector<MTransform*> _transforms;
	string _identifier;
	void MeshHandler();
	void TransformHandler();
	void LightHandler();
	void MaterialHandler();
	void CameraHandler();

	void ReadIdentifier();
};