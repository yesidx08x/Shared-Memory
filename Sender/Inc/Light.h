#pragma once
#include "Object.h"

class Light : public Object
{
public:
	Light();
	~Light();

	void SetData(MObject& object, PackageType packageType);
	void SendPackage(void* buffer, unsigned int head);

private:
	LightHeader _header;
	LightData _data;
};