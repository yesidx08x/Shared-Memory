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
		ID3D11Buffer* buffer = nullptr;
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
	void UpdateBuffer(ID3D11Buffer* buffer, void* data, unsigned int size);

	ID3D11Buffer** GetTransform();
	ID3D11Buffer** GetCamera();
	ID3D11Buffer** GetCameraInfo();
	ID3D11Buffer** GetPBRChoice();

private:
	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _devcon = nullptr;

	vector<Buffer> _meshBuffers;
	vector<Buffer> _lightBuffers;
	ID3D11Buffer* _lightBuffer = nullptr;
	LightBuffer _lightDatas[LighBufferSize];

	ID3D11Buffer* _transformBuffer = nullptr;
	ID3D11Buffer* _cameraBuffer = nullptr;
	ID3D11Buffer* _cameraInfoBuffer = nullptr;
	ID3D11Buffer* _pbrChoiceBuffer = nullptr;
	ID3D11Buffer* _nullBuffer = nullptr;

	ID3D11Buffer* CreateBuffer(unsigned int size, void* data, string name, D3D11_BIND_FLAG bindFlag);
};