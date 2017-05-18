#pragma once
#include "MObject.h"

class MTransform : public MObject
{
public:
	MTransform();
	MTransform(void* buffer, unsigned int& tail);
	~MTransform();

	void ReadData(void* buffer, unsigned int& tail, PackageType packageType = PACKAGETYPEAMOUNT);
	TransformData* GetData();

private:
	TransformData* _data;
	TransformHeader _header;
	MatrixData _matrixData;
};