#pragma once
#include "Object.h"

class Material : public Object
{
public:
	Material();
	~Material();

	void SetData(MObject& object, PackageType packageType);
	void SendPackage(void* buffer, unsigned int head);

private:
	MaterialHeader _header;
	ColorData _data;
	string _albedo;
	string _roughness;
	string _metallic;
	string _normal;
	string _displacement;
	string _radience;
	string _irradience;

	void GetWhole(MObject& object);
	void GetTextureName(MObject& object);

	void SendWhole(void* buffer, unsigned int head);
	void SendTextureName(void* buffer, unsigned int head);
};