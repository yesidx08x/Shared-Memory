#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include <string>
#include "DataStructures.h"

using namespace DirectX;
using namespace std;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class BufferHandler
{
public:
	struct Buffer
	{
		ID3D11Buffer* buffer;
		string identifier;
	};
	BufferHandler();
	BufferHandler(ID3D11Device* device, ID3D11DeviceContext* devcon);
	~BufferHandler();

	void CreateMeshBuffer(MeshData& data);
	void RemoveMeshBuffer(string identifier);
	void UpdateMeshBuffer(MeshData& data);
	ID3D11Buffer** GetVertexBuffer(int id);

	void UpdateLightBuffer(vector<LightBuffer> data);
	ID3D11Buffer** GetLightBuffers();
	unsigned int GetLightBufferSize();

	ID3D11Buffer* CreateConstantBuffer(unsigned int size, void* data, string name);
	ID3D11Buffer* CreateVertexBuffer(unsigned int size, void* data, string name);
	void UpdateBuffer(ID3D11Buffer* buffer, void* data, unsigned int size);

	ID3D11Buffer** GetTransform();
	ID3D11Buffer** GetCamera();
	ID3D11Buffer** GetCameraInfo();
	ID3D11Buffer** GetPBRChoice();

private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _devcon;

	vector<Buffer> _meshBuffers;
	vector<Buffer> _lightBuffers;
	ID3D11Buffer* _lightBuffer;
	LightBuffer _lightDatas[LighBufferSize];

	ID3D11Buffer* _transformBuffer;
	ID3D11Buffer* _cameraBuffer;
	ID3D11Buffer* _cameraInfoBuffer;
	ID3D11Buffer* _pbrChoiceBuffer;
	ID3D11Buffer* _nullBuffer = nullptr;

	string _path;
	void CreateStandardBuffers();
};