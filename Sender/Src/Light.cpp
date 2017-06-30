#include "Light.h"

Light::Light()
{
	_data.buffer = new LightBuffer();
}

Light::~Light()
{
	delete _data.buffer;
}

void Light::SetData(MObject& object, PackageType packageType)
{
	_identifierSize = 0;
	_packageType = packageType;
	_size = 0;

	MFnLight light(object);
	SetIdentifier(light.name());
	SetTransformID(light.parent(0));
	_header.transformIDSize = SetTransformIdentifier(light.parent(0));

	MColor color;
	color = light.color();
	_data.buffer->color = XMFLOAT4(color.r, color.g, color.b, color.a);

	if (object.hasFn(MFn::kPointLight))
		_header.type = (unsigned int)point;
	else if (object.hasFn(MFn::kDirectionalLight))
		_header.type = (unsigned int)directional;

	_size += sizeof(LightHeader);
	_size += sizeof(LightBuffer);
	_size += _header.transformIDSize;
}

void Light::SendPackage(void* buffer, unsigned int head)
{
	Print("LIGHT");
	// Identifier
	memcpy((char*)buffer + head, _identifier.data(), _identifierSize);
	head += _identifierSize;
	// Header
	memcpy((char*)buffer + head, &_header, sizeof(LightHeader));
	head += sizeof(LightHeader);
	// Transform ID
	memcpy((char*)buffer + head, _transformIdentifier.data(), _header.transformIDSize);
	head += _header.transformIDSize;
	// Data
	memcpy((char*)buffer + head, &_data, sizeof(LightBuffer));
	head += sizeof(LightBuffer);
}