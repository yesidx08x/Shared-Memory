#pragma once
#include "MMesh.h"
#include "MSkinnedMesh.h"
#include "MSkinCluster.h"
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
	vector<MSkinnedMesh*>* GetSkinnedMeshes();
	vector<MSkinCluster*>* GetSkinClusters();
	vector<MTransform*>* GetTransforms();
	vector<MLight*>* GetLights();
	vector<MMaterial*>* GetMaterials();
	string GetIdentifier();
	MCamera* GetCamera();
	UINT32* GetMeshStrides();
	UINT32* GetMeshOffsets();
	UINT32* GetSkinnedMeshStrides();
	UINT32* GetSkinnedMeshOffsets();

private:
	SharedMemory _sharedMemory;
	SharedMemory::CircInfo _circInfo;
	PackageType _packageType;

	MessageHeader _messageHeader;

	UINT32 meshStrides = 12 + 8 + 12 + 12;
	UINT32 meshOffsets = 0;
	UINT32 skinnedMeshStrides = 12 + 8 + 12 + 16 + 16;
	UINT32 skinnedMeshOffsets = 0;

	MCamera _camera;
	vector<MMesh*> _meshes;
	vector<MSkinnedMesh*> _skinnedMeshes;
	vector<MSkinCluster*> _skinClusters;
	vector<MLight*> _lights;
	vector<MMaterial*> _materials;
	vector<MTransform*> _transforms;
	string _identifier;
	void MeshHandler();
	void SkinnedMeshHandler();
	void SkinClusterHandler();
	void TransformHandler();
	void LightHandler();
	void MaterialHandler();
	void CameraHandler();

	void ReadIdentifier();
};