#pragma once
#include "Object.h"

class Transform : public Object
{
public:
	Transform();
	~Transform();

	void SetData(MObject& object, PackageType packageType);
	void SendPackage(void* buffer, unsigned int head);
private:
	TransformData _data;
};