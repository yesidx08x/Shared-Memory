#include "../Receiver/Inc/MLight.h"

MLight::MLight(){}

MLight::MLight(void * buffer, unsigned int & tail)
{
	_header = new LightHeader();
	_data = new LightData();
	ReadData(buffer, tail);
}

MLight::~MLight() 
{
	_data->buffer == nullptr ? 0 : delete _data->buffer;
	delete _header;
	delete _data;
}

void MLight::ReadData(void* buffer, unsigned int& tail, PackageType packageType)
{
	// Header
	memcpy(_header, (char*)buffer + tail, sizeof(LightHeader));
	tail += sizeof(LightHeader);
	// Transform Identifier
	_transformIdentifier.resize(_header->transformIDSize);
	memcpy((char*)_transformIdentifier.data(), (char*)buffer + tail, _header->transformIDSize);
	tail += _header->transformIDSize;
	// Data
	_data->buffer = new LightBuffer();
	memcpy(_data->buffer, (char*)buffer + tail, sizeof(LightBuffer));
	tail += sizeof(LightBuffer);
	_data->type = (LightType)_header->type;
}

LightData* MLight::GetData() {
	return _data; }