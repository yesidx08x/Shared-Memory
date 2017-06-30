#pragma once
#include "MayaHeaders.h"
#include "DataStructures.h"

class Object
{
public:
	Object();
	~Object();

	unsigned int GetID();
	unsigned int GetPackageSize();
	unsigned int GetIdentifierSize();
	PackageType GetPackageType();
	unsigned int SetTransformID(MObject& object);
	unsigned int SetMaterialID(MObject& object);

	unsigned int SetTransformIdentifier(MObject& object);
	unsigned int SetMaterialIdentifier(MObject& object);
	unsigned int SetIdentifier(MString name);

	virtual void SetData(MObject& object, PackageType packageType) = 0;
	virtual void SendPackage(void* buffer, unsigned int head) = 0;

	void SendTransformID(void* buffer, unsigned int head);
	void SendMaterialID(void* buffer, unsigned int head);
	void SendIdentifier(void* buffer, unsigned int head);
protected:

	unsigned int _size;
	unsigned int _id;
	unsigned int _identifierSize;
	PackageType _packageType;
	int _transformId;
	int _materialId;

	string _transformIdentifier;
	string _materialIdentifier;
	string _identifier;

	MStatus _status;
};