#pragma once
#include "Object.h"

class Camera : public Object
{
public:
	Camera();
	~Camera();

	void SetData(MObject& object, PackageType packageType);
	void SendPackage(void* buffer, unsigned int head);

private:
	CameraData _data;
};