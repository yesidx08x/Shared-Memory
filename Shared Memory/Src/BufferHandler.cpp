#include "BufferHandler.h"

BufferHandler::BufferHandler(){}

BufferHandler::BufferHandler(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	_device = device;
	_devcon = devcon;
	
	// Standard buffers
	_transformBuffer = CreateBuffer(sizeof(XMFLOAT4X4), &XMFLOAT4X4(), "Standard Tranform", D3D11_BIND_CONSTANT_BUFFER);
	_cameraBuffer = CreateBuffer(sizeof(CameraBuffers), &CameraBuffers(), "Standard Camera", D3D11_BIND_CONSTANT_BUFFER);
	_cameraInfoBuffer = CreateBuffer(sizeof(CameraInfo), &CameraInfo(), "Standard Camera Info", D3D11_BIND_CONSTANT_BUFFER);
	_lightBuffer = CreateBuffer(sizeof(LightBuffer) * LighBufferSize, &LightBuffer(), "Standard LightBuffer", D3D11_BIND_CONSTANT_BUFFER);
}

BufferHandler::~BufferHandler()
{
	for (size_t i = 0; i < _meshBuffers.size(); i++)
		_meshBuffers[i].buffer->Release();

	for (size_t i = 0; i < _lightBuffers.size(); i++)
		_lightBuffers[i].buffer->Release();

	_lightBuffer->Release();
	_transformBuffer->Release();
	_cameraBuffer->Release();
	_cameraInfoBuffer->Release();
}

void BufferHandler::CreateMeshBuffer(MeshData& data)
{
	for (unsigned int i = 0; i < _meshBuffers.size(); i++)
	{
		if (data.identifier == _meshBuffers[i].identifier) // Look for already created buffers
		{
			return;
		}
	}

	_meshBuffers.push_back(Buffer());
	if(data.vertexSize == sizeof(VertexData))
		_meshBuffers.back().buffer = CreateBuffer(data.vertexCount * data.vertexSize, data.vertexData, data.identifier, D3D11_BIND_VERTEX_BUFFER);
	else if (data.vertexSize == sizeof(SmallVertexData))
		_meshBuffers.back().buffer = CreateBuffer(data.vertexCount * data.vertexSize, data.smallVertexData, data.identifier, D3D11_BIND_VERTEX_BUFFER);
	else if (data.vertexSize == sizeof(XMFLOAT4))
		_meshBuffers.back().buffer = CreateBuffer(data.vertexCount * data.vertexSize, data.position, data.identifier, D3D11_BIND_VERTEX_BUFFER);
	_meshBuffers.back().identifier = data.identifier;
}

ID3D11Buffer* BufferHandler::CreateBuffer(unsigned int size, void* data, string name, D3D11_BIND_FLAG bindFlag)
{
	// Create a constant or vertex buffer based on bindFlag
	ID3D11Buffer* buffer = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = bindFlag;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = size;

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = data;

	HRESULT hr = _device->CreateBuffer(&bufferDesc, &subData, &buffer);
	if (hr != S_OK)
		PrintError("Error when creating buffer \"" + name + "\"");
	return buffer;
}

void BufferHandler::RemoveMeshBuffer(string identifier)
{
	// Completely remove buffer from pipeline in DirectX
	for (size_t i = 0; i < _meshBuffers.size(); i++)
	{
		if (_meshBuffers[i].identifier == identifier)
		{
			_meshBuffers[i].buffer->Release();
			break;
		}
	}
}

void BufferHandler::UpdateMeshBuffer(MeshData & data)
{
	for (size_t i = 0; i < _meshBuffers.size(); i++)
	{
		if (_meshBuffers[i].identifier == data.identifier)
		{
			UpdateBuffer(_meshBuffers[i].buffer, data.vertexData, data.vertexCount * data.vertexSize);
			data.update = false;
			break;
		}
	}
}

void BufferHandler::UpdateLightBuffer(vector<LightBuffer> data)
{
	memcpy(_lightDatas, data.data(), sizeof(LightBuffer) * data.size());
	UpdateBuffer(_lightBuffer, _lightDatas, sizeof(LightBuffer) * LighBufferSize);
}

void BufferHandler::UpdateBuffer(ID3D11Buffer* buffer, void* data, unsigned int size)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	_devcon->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, (char*)data, size);
	_devcon->Unmap(buffer, 0);
}

ID3D11Buffer** BufferHandler::GetVertexBuffer(int id){return &_meshBuffers[id].buffer;}

ID3D11Buffer** BufferHandler::GetLightBuffers() { return &_lightBuffer;}

ID3D11Buffer** BufferHandler::GetTransform() { return &_transformBuffer; }

ID3D11Buffer** BufferHandler::GetCamera() { return &_cameraBuffer; }

ID3D11Buffer** BufferHandler::GetCameraInfo() { return &_cameraInfoBuffer; }