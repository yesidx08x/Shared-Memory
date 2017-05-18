#include "../Receiver/Inc/MMesh.h"

MMesh::MMesh(){}

unsigned int MMesh::GetVertexCount() { return _data->vertexCount; }

MeshData* MMesh::GetData() { return _data; }

vector<MaterialData> MMesh::GetMaterialData(){return _materialData;}

MMesh::MMesh(void* buffer, unsigned int& tail)
{
	_data = new MeshData();
	ReadWhole(buffer, tail);
}

MMesh::~MMesh(){}

void MMesh::ReadData(void* buffer, unsigned int& tail, PackageType packageType)
{
	switch (packageType)
	{
	case WHOLE:
	case CREATE:
		ReadWhole(buffer, tail);
		break;
	case TRANSFORMID:
		ReadTransformID(buffer, tail);
		break;
	case MATERIALID:
		ReadMaterialID(buffer, tail);
		break;
	case DESTROY:
		//ReadIndentifier(buffer, tail);
		break;
	default:
		break;
	}
}

void MMesh::ReadWhole(void* buffer, unsigned int& tail)
{
	// Header
	memcpy(&_header, (char*)buffer + tail, sizeof(MeshHeader));
	tail += sizeof(MeshHeader);
	_transformId = _header.transformID;
	_materialId = _header.materialID;
	_data->vertexCount = _header.vertexCount;
	_data->vertexData = new VertexData[_data->vertexCount];

	// Identifier
	_data->identifier.resize(_header.identifierSize);
	memcpy((char*)_data->identifier.data(), (char*)buffer + tail, _header.identifierSize);
	tail += _header.identifierSize;
	// Transform Identifier
	_transformIdentifier.resize(_header.transformIDSize);
	memcpy((char*)_transformIdentifier.data(), (char*)buffer + tail, _header.transformIDSize);
	tail += _header.transformIDSize;
	// Materials
	for (size_t i = 0; i < _header.materialIDSize; i++)
	{
		_materialData.push_back(MaterialData());
		memcpy(&_materialData.back().type, (char*)buffer + tail, sizeof(unsigned int));
		tail += sizeof(unsigned int);
		unsigned int size;
		memcpy(&size, (char*)buffer + tail, sizeof(unsigned int));
		tail += sizeof(unsigned int);
		_materialData.back().identifier.resize(size);
		memcpy((char*)_materialData.back().identifier.data(), (char*)buffer + tail, size);
		_materialData.back().inMap = false;
		tail += size;
	}
	// Data
	memcpy(_data->vertexData, (char*)buffer + tail, sizeof(VertexData) * _data->vertexCount);
	tail += sizeof(VertexData) * _data->vertexCount;
}

void MMesh::ReadIndentifier(void * buffer, unsigned int & tail)
{
	// Header
	memcpy(&_header, (char*)buffer + tail, sizeof(MeshHeader));
	tail += sizeof(MeshHeader);

	// Identifier
	_data->identifier.resize(_header.identifierSize);
	memcpy((char*)_data->identifier.data(), (char*)buffer + tail, _header.identifierSize);
	tail += _header.identifierSize;
}