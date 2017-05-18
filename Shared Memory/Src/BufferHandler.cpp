#include "BufferHandler.h"

BufferHandler::BufferHandler(){}

BufferHandler::BufferHandler(ID3D11Device * device, ID3D11DeviceContext * devcon)
{
	_device = device;
	_devcon = devcon;
	_path = "GRF/Standards/";
	CreateStandardBuffers();
}

BufferHandler::~BufferHandler(){}

void BufferHandler::CreateMeshBuffer(MeshData & data)
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
		_meshBuffers.back().buffer = CreateVertexBuffer(data.vertexCount * data.vertexSize, data.vertexData, data.identifier);
	else if (data.vertexSize == sizeof(SmallVertexData))
		_meshBuffers.back().buffer = CreateVertexBuffer(data.vertexCount * data.vertexSize, data.smallVertexData, data.identifier);
	else if (data.vertexSize == sizeof(XMFLOAT4))
		_meshBuffers.back().buffer = CreateVertexBuffer(data.vertexCount * data.vertexSize, data.position, data.identifier);
	_meshBuffers.back().identifier = data.identifier;
}

void BufferHandler::RemoveMeshBuffer(string identifier)
{
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

ID3D11Buffer** BufferHandler::GetVertexBuffer(int id){return &_meshBuffers[id].buffer;}

ID3D11Buffer** BufferHandler::GetLightBuffers() 
{ 
	return &_lightBuffer;
}

unsigned int BufferHandler::GetLightBufferSize(){return _lightBuffers.size();}

ID3D11Buffer* BufferHandler::CreateConstantBuffer(unsigned int size, void* data, string name)
{
	ID3D11Buffer* buffer = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = size;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = data;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	_device->CreateBuffer(&bufferDesc, &subData, &buffer);
	HRESULT hr = _device->CreateBuffer(&bufferDesc, &subData, &buffer);
	if (hr != S_OK)
		PrintError("Error when creating constant buffer \"" + name + "\"");
	return buffer;
}

ID3D11Buffer * BufferHandler::CreateVertexBuffer(unsigned int size, void * data, string name)
{
	ID3D11Buffer* buffer = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = size;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = data;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	HRESULT hr = _device->CreateBuffer(&bufferDesc, &subData, &buffer);
	if (hr != S_OK)
		PrintError("Error when creating vertex buffer  \"" + name + "\"");
	return buffer;
}

void BufferHandler::UpdateBuffer(ID3D11Buffer* buffer, void* data, unsigned int size)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	_devcon->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, (char*)data, size);
	_devcon->Unmap(buffer, 0);
}

ID3D11Buffer ** BufferHandler::GetTransform(){return &_transformBuffer;}

ID3D11Buffer ** BufferHandler::GetCamera(){return &_cameraBuffer;}

ID3D11Buffer ** BufferHandler::GetCameraInfo(){return &_cameraInfoBuffer;}

ID3D11Buffer ** BufferHandler::GetPBRChoice(){return &_pbrChoiceBuffer;}

void BufferHandler::CreateStandardBuffers()
{
	_transformBuffer = CreateConstantBuffer(sizeof(XMFLOAT4X4), &XMFLOAT4X4(), "Standard Tranform");
	_cameraBuffer = CreateConstantBuffer(sizeof(CameraBuffers), &CameraBuffers(), "Standard Camera");
	_cameraInfoBuffer = CreateConstantBuffer(sizeof(CameraInfo), &CameraInfo(), "Standard Camera Info");
	_pbrChoiceBuffer = CreateConstantBuffer(sizeof(PBRChoice), &PBRChoice(), "Standard PBRChoice");
	_lightBuffer = CreateConstantBuffer(sizeof(LightData) * LighBufferSize, &LightData(), "Standard LightBuffer");
}