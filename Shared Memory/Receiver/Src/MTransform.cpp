#include "../Receiver/Inc/MTransform.h"

MTransform::MTransform(){}

MTransform::MTransform(void* buffer, unsigned int & tail)
{
	_data = new TransformData();
	ReadData(buffer, tail);
}

MTransform::~MTransform()
{
	delete _data;
}

void MTransform::ReadData(void * buffer, unsigned int & tail, PackageType packageType)
{
	// Data
	memcpy(&_data->transform, (char*)buffer + tail, sizeof(MatrixData));
	tail += sizeof(MatrixData);
}

TransformData* MTransform::GetData(){return _data;}