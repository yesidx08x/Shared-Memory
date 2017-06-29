#include "../Receiver/Inc/MDataHandler.h"

MDataHandler::MDataHandler()
{
	_sharedMemory.OpenMemory(10, false);
}

MDataHandler::~MDataHandler()
{
	_sharedMemory.CloseMemory();
}

vector<MMesh*>* MDataHandler::GetMeshes() { return &_meshes; }

vector<MTransform*>* MDataHandler::GetTransforms() { return &_transforms; }

vector<MLight*>* MDataHandler::GetLights() { return &_lights; }

vector<MMaterial*>* MDataHandler::GetMaterials(){return &_materials;}

string MDataHandler::GetIdentifier(){return _identifier;}

MCamera* MDataHandler::GetCamera() { return &_camera; }

UINT32* MDataHandler::GetMeshStrides(){return &meshStrides;}

UINT32* MDataHandler::GetMeshOffsets(){return &meshOffsets;}

DataType MDataHandler::Update()
{
	// Get buffer info
	_circInfo = _sharedMemory.GetInfo();
	if (_circInfo.freeMem < _circInfo.totalMem)
	{
		memcpy(&_messageHeader, (char*)_sharedMemory._buffer + _circInfo.tail, sizeof(MessageHeader));
		_circInfo.tail += sizeof(MessageHeader);
		_packageType = (PackageType)_messageHeader.subType;

		if (_messageHeader.size < _circInfo.freeMem)
		{
			switch (_messageHeader.type)
			{
			case CAMERA:
				CameraHandler();
				break;
			case TRANSFORM:
				TransformHandler();
				break;
			case MESH:
				MeshHandler();
				break;
			case LIGHT:
				LightHandler();
				break;
			case MATERIAL:
				MaterialHandler();
				break;
			default:
				break;
			}
		}
		// Set buffer info
		_circInfo.freeMem += _messageHeader.size + sizeof(MessageHeader);
		_circInfo.ReadCount++;
		_sharedMemory.SetInfo(_circInfo);
		return (DataType)_messageHeader.type;
	}
	return DATATYPEAMOUNT;
}

bool MDataHandler::SharedOpen(){return _sharedMemory.IsOpen();}

MessageHeader MDataHandler::GetMessageHeader(){return _messageHeader;}

void MDataHandler::MeshHandler()
{
	if (_packageType == DESTROY)
	{
		ReadIdentifier();
		delete _meshes[_messageHeader.id];
		_meshes[_messageHeader.id] = nullptr;
	}
	else if (_packageType == CREATE)
		_meshes.push_back(new MMesh(_sharedMemory._buffer, _circInfo.tail));
	else
		_meshes[_messageHeader.id]->ReadData(_sharedMemory._buffer, _circInfo.tail, _packageType);
}

void MDataHandler::TransformHandler()
{
	if (_packageType == DESTROY)
	{
		ReadIdentifier();
		delete _transforms[_messageHeader.id];
		_transforms[_messageHeader.id] = nullptr;
	}
	else if (_packageType == CREATE)
		_transforms.push_back(new MTransform(_sharedMemory._buffer, _circInfo.tail));
	else
		_transforms[_messageHeader.id]->ReadData(_sharedMemory._buffer, _circInfo.tail, _packageType);
}

void MDataHandler::LightHandler()
{
	if (_packageType == DESTROY)
	{
		ReadIdentifier();
		delete _lights[_messageHeader.id];
		_lights[_messageHeader.id] = nullptr;
	}
	else if (_packageType == CREATE)
		_lights.push_back(new MLight(_sharedMemory._buffer, _circInfo.tail));
	else
		_lights[_messageHeader.id]->ReadData(_sharedMemory._buffer, _circInfo.tail, _packageType);
}

void MDataHandler::MaterialHandler()
{
	if (_packageType == DESTROY)
	{
		ReadIdentifier();
		delete _materials[_messageHeader.id];
		_materials[_messageHeader.id] = nullptr;
	}
	else if (_packageType == CREATE)
		_materials.push_back(new MMaterial(_sharedMemory._buffer, _circInfo.tail));
	else
		_materials[_messageHeader.id]->ReadData(_sharedMemory._buffer, _circInfo.tail, _packageType);
}

void MDataHandler::CameraHandler()
{
	_camera.ReadData(_sharedMemory._buffer, _circInfo.tail, _packageType);
}

void MDataHandler::ReadIdentifier()
{
	_identifier.clear();
	_identifier.resize(_messageHeader.size);
	memcpy((char*)_identifier.data(), (char*)_sharedMemory._buffer + _circInfo.tail, _messageHeader.size);
	_circInfo.tail += _messageHeader.size;
}

void MDataHandler::ShutDown()
{
	_meshes.clear();
	_lights.clear();
	_materials.clear();
	_transforms.clear();
}