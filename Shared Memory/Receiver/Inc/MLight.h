#pragma once
#include "MObject.h"

class MLight : public MObject
{
public:
	MLight();
	MLight(void* buffer, unsigned int& tail);;
	~MLight();

	void ReadData(void* buffer, unsigned int& tail, PackageType packageType = PACKAGETYPEAMOUNT);
	LightData* GetData();

private:
	LightHeader* _header;
	LightData* _data;
};