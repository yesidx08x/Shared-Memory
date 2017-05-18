#include "../Receiver/Inc/MTransform.h"

MTransform::MTransform(){}

MTransform::MTransform(void* buffer, unsigned int & tail)
{
	_data = new TransformData();
	ReadData(buffer, tail);
}

MTransform::~MTransform(){}

void MTransform::ReadData(void * buffer, unsigned int & tail, PackageType packageType)
{
	// Header
	memcpy(&_header, (char*)buffer + tail, sizeof(TransformHeader));
	tail += sizeof(TransformHeader);
	// Identifier
	_data->identifier.resize(_header.identifierSize);
	memcpy((char*)_data->identifier.data(), (char*)buffer + tail, _header.identifierSize);
	tail += _header.identifierSize;
	// Data
	memcpy(&_matrixData, (char*)buffer + tail, sizeof(MatrixData));
	tail += sizeof(MatrixData);
	_data->position = _matrixData.position;
	_data->scale = _matrixData.scale;
	_data->rotation = _matrixData.rotation;
}

TransformData* MTransform::GetData(){return _data;}