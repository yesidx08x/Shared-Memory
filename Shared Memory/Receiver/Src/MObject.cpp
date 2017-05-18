#include "../Receiver/Inc/MObject.h"

MObject::MObject(){}

MObject::~MObject(){}

ID3D11Buffer** MObject::GetBuffer(){
	return &_bufferData;}

void MObject::ReleaseBuffer(){SafeRelease(_bufferData);}

void MObject::SetBuffer(ID3D11Buffer* buffer){_bufferData = buffer;}

unsigned int MObject::GetTransformId(){return _transformId;}

string MObject::GetTransformID(){return _transformIdentifier;}

unsigned int MObject::GetMaterialId(){ return _materialId; }

unsigned int MObject::GetTextureId() { return _textureId; }

unsigned int MObject::GetSkinClusterId(){return _skinClusterId;}

void MObject::ReadTransformID(void * buffer, unsigned int & tail)
{
	memcpy(&_transformId, (char*)buffer + tail, sizeof(unsigned int));
	tail += sizeof(unsigned int);
}

void MObject::ReadMaterialID(void * buffer, unsigned int & tail)
{
	memcpy(&_materialId, (char*)buffer + tail, sizeof(unsigned int));
	tail += sizeof(unsigned int);
}