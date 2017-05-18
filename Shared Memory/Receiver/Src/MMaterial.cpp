#include "../Receiver/Inc/MMaterial.h"

MMaterial::MMaterial(){}

MMaterial::MMaterial(void * buffer, unsigned int & tail)
{
	_textureNames.resize(7);
	ReadWhole(buffer, tail);
}

MMaterial::~MMaterial(){}

vector<MaterialData> MMaterial::GetData() { return _data; }

void MMaterial::ReadData(void * buffer, unsigned int & tail, PackageType packageType)
{
	_packageType = packageType;
	switch (_packageType)
	{
	case WHOLE:
		ReadWhole(buffer, tail);
		break;
	case CREATE:
		ReadWhole(buffer, tail);
		break;
	case TEXNAME:
		ReadTextureName(buffer, tail);
		break;
	default:
		break;
	}
}

void MMaterial::ReadWhole(void * buffer, unsigned int & tail)
{
	// Header
	memcpy(&_header, (char*)buffer + tail, sizeof(MaterialHeader));
	tail += sizeof(MaterialHeader);
	// Data
	if (_header.albedoSize > 0)
	{
		_data.push_back(MaterialData());
		_data.back().type = albedo;
		_data.back().identifier.resize(_header.albedoSize);
		memcpy((char*)_data.back().identifier.data(), (char*)buffer + tail, _header.albedoSize);
		tail += _header.albedoSize;
	}
	if (_header.roughnessSize > 0)
	{
		_data.push_back(MaterialData());
		_data.back().type = roughness;
		_data.back().identifier.resize(_header.roughnessSize);
		memcpy((char*)_data.back().identifier.data(), (char*)buffer + tail, _header.roughnessSize);
		tail += _header.roughnessSize;
	}
	if (_header.metallicSize > 0)
	{
		_data.push_back(MaterialData());
		_data.back().type = metallic;
		_data.back().identifier.resize(_header.metallicSize);
		memcpy((char*)_data.back().identifier.data(), (char*)buffer + tail, _header.metallicSize);
		tail += _header.metallicSize;
	}
	if (_header.normalSize > 0)
	{
		_data.push_back(MaterialData());
		_data.back().type = normal;
		_data.back().identifier.resize(_header.normalSize);
		memcpy((char*)_data.back().identifier.data(), (char*)buffer + tail, _header.normalSize);
		tail += _header.normalSize;
	}
	if (_header.displacementSize > 0)
	{
		_data.push_back(MaterialData());
		_data.back().type = displacement;
		_data.back().identifier.resize(_header.displacementSize);
		memcpy((char*)_data.back().identifier.data(), (char*)buffer + tail, _header.displacementSize);
		tail += _header.displacementSize;
	}
	if (_header.radienceSize > 0)
	{
		_data.push_back(MaterialData());
		_data.back().type = radience;
		_data.back().identifier.resize(_header.radienceSize);
		memcpy((char*)_data[0].identifier.data(), (char*)buffer + tail, _header.radienceSize);
		tail += _header.radienceSize;
	}
	if (_header.irradienceSize > 0)
	{
		_data.push_back(MaterialData());
		_data.back().type = irradiance;
		_data.back().identifier.resize(_header.irradienceSize);
		memcpy((char*)_data.back().identifier.data(), (char*)buffer + tail, _header.irradienceSize);
		tail += _header.irradienceSize;
	}
}

void MMaterial::ReadTextureName(void * buffer, unsigned int & tail)
{
	// Texture path
	_textureNames[0].resize(_header.albedoSize);
	memcpy((char*)_textureNames[0].data(), (char*)buffer + tail, _header.albedoSize);
	tail += _header.albedoSize;
}