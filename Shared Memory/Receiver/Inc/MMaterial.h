#pragma once
#include "MObject.h"

class MMaterial : public MObject
{
public:
	MMaterial();
	MMaterial(void* buffer, unsigned int& tail);
	~MMaterial();

	void ReadData(void* buffer, unsigned int& tail, PackageType packageType = PACKAGETYPEAMOUNT);
	vector<MaterialData> GetData();

private:
	MaterialHeader _header;
	vector<MaterialData> _data;
	vector<string> _textureNames;

	void ReadWhole(void* buffer, unsigned int& tail);
	void ReadTextureName(void* buffer, unsigned int& tail);
};