#include "../Receiver/Inc/MLight.h"

MLight::MLight(){}

MLight::MLight(void * buffer, unsigned int & tail)
{
	_header = new LightHeader();
	_data = new LightData();
	ReadData(buffer, tail);
}

MLight::~MLight() {}

void MLight::ReadData(void* buffer, unsigned int& tail, PackageType packageType)
{
	// Header
	memcpy(_header, (char*)buffer + tail, sizeof(LightHeader));
	tail += sizeof(LightHeader);
	// Identifier
	_data->identifier.resize(_header->identifierSize);
	memcpy((char*)_data->identifier.data(), (char*)buffer + tail, _header->identifierSize);
	tail += _header->identifierSize;
	// Transform Identifier
	_transformIdentifier.resize(_header->transformIDSize);
	memcpy((char*)_transformIdentifier.data(), (char*)buffer + tail, _header->transformIDSize);
	tail += _header->transformIDSize;
	// Data
	memcpy(&_data->buffer, (char*)buffer + tail, sizeof(LightBuffer));
	tail += sizeof(LightBuffer);
	_data->type = (LightType)_header->type;
}

LightData* MLight::GetData() { return _data; }