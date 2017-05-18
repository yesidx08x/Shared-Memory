#pragma once
#include "MObject.h"

class MCamera : public MObject
{
public:
	MCamera();
	~MCamera();

	void ReadData(void* buffer, unsigned int& tail, PackageType packageType = PACKAGETYPEAMOUNT);
	CameraData* GetData();

private:
	CameraData* _data;

	void ReadWhole(void* buffer, unsigned int& tail);
};