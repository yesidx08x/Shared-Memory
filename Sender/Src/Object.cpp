#include "Object.h"

Object::Object(){}

Object::~Object(){}

unsigned int Object::GetID() { return _id; }

unsigned int Object::GetPackageSize() { return _size; }

unsigned int Object::GetIdentifierSize() { return _identifierSize; }

PackageType Object::GetPackageType(){return _packageType;}

unsigned int Object::SetTransformID(MObject& object)
{
	MFnTransform transform(object);
	MFnNumericAttribute attribute(transform.attribute("SharedMemoryID", &_status));
	Check_Error(_status, transform.name());
	attribute.getDefault(_transformId);
	return _transformId;
}

unsigned int Object::SetMaterialID(MObject & object)
{
	MFnDependencyNode depNode(object);
	MFnNumericAttribute attribute(depNode.attribute("SharedMemoryID", &_status));
	Check_Error(_status, depNode.name());
	attribute.getDefault(_materialId);
	return _materialId;
}

unsigned int Object::SetTransformIdentifier(MObject & object)
{
	MFnTransform transform(object);
	_transformIdentifier = transform.name().asChar();
	return _transformIdentifier.size();
}

unsigned int Object::SetMaterialIdentifier(MObject & object)
{
	MFnDependencyNode depNode(object);
	_materialIdentifier = depNode.name().asChar();
	return _materialIdentifier.size();
}

unsigned int Object::SetIdentifier(MString name)
{
	_identifier = name.asChar();
	_identifierSize = name.length();
	return _identifier.size();
}

void Object::SendTransformID(void * buffer, unsigned int head)
{
	memcpy((char*)buffer + head, &_transformId, sizeof(unsigned int));
}

void Object::SendMaterialID(void * buffer, unsigned int head)
{
	memcpy((char*)buffer + head, &_materialId, sizeof(unsigned int));
}

void Object::SendIdentifier(void * buffer, unsigned int head)
{
	memcpy((char*)buffer + head, _identifier.data(), _identifier.size());
}