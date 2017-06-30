#include "../Receiver/Inc/MDataHandler.h"

MDataHandler::MDataHandler()
{
	_sharedMemory.OpenMemory(10, false);
}

MDataHandler::~MDataHandler()
{
	_sharedMemory.CloseMemory();
}

MMesh* MDataHandler::GetMeshe(string identifier)
{
	MMesh* mesh = nullptr;
	try
	{
		mesh = _meshes.at(identifier);
	}
	catch (const std::out_of_range& e)
	{
#ifdef _DEBUG
		PrintError("Could not find entity \"" + identifier + " " + e.what() + "\"");
#endif
		return nullptr;
	}

	return mesh;
}

MTransform* MDataHandler::GetTransform(string identifier)
{
	MTransform* transform = nullptr;
	try
	{
		transform = _transforms.at(identifier);
	}
	catch (const std::out_of_range& e)
	{
#ifdef _DEBUG
		PrintError("Could not find entity \"" + identifier + " " + e.what() + "\"");
#endif
		return nullptr;
	}

	return transform;
}

MLight* MDataHandler::GetLight(string identifier)
{
	MLight* light = nullptr;
	try
	{
		light = _lights.at(identifier);
	}
	catch (const std::out_of_range& e)
	{
#ifdef _DEBUG
		PrintError("Could not find entity \"" + identifier + " " + e.what() + "\"");
#endif
		return nullptr;
	}

	return light;
}

MMaterial* MDataHandler::GetMaterial(string identifier)
{
	MMaterial* material = nullptr;
	try
	{
		material = _materials.at(identifier);
	}
	catch (const std::out_of_range& e)
	{
#ifdef _DEBUG
		PrintError("Could not find entity \"" + identifier + " " + e.what() + "\"");
#endif
		return nullptr;
	}

	return material;
}

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
			if (_messageHeader.identifierSize > 0)
			{
				_identifier.clear();
				_identifier.resize(_messageHeader.identifierSize);
				memcpy((char*)_identifier.data(), (char*)_sharedMemory._buffer + _circInfo.tail, _messageHeader.identifierSize);
				_circInfo.tail += _messageHeader.identifierSize;
			}
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
		_circInfo.freeMem += _messageHeader.size + sizeof(MessageHeader) + _messageHeader.identifierSize;
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
		delete _meshes.at(_identifier);
		_meshes.at(_identifier) = nullptr;
	}
	else if (_packageType == CREATE)
	{
		MMesh* mesh = new MMesh(_sharedMemory._buffer, _circInfo.tail);
		_meshes.insert(pair<string, MMesh*>(_identifier, mesh));
		_meshes[_identifier]->GetData()->identifier = _identifier;
	}
	else
		_meshes.at(_identifier)->ReadData(_sharedMemory._buffer, _circInfo.tail, _packageType);
}

void MDataHandler::TransformHandler()
{
	if (_packageType == DESTROY)
	{
		delete _transforms.at(_identifier);
		_transforms.at(_identifier) = nullptr;
	}
	else if (_packageType == CREATE)
	{
		MTransform* transform = new MTransform(_sharedMemory._buffer, _circInfo.tail);
		_transforms.insert(pair<string, MTransform*>(_identifier, transform));
		_transforms[_identifier]->GetData()->identifier = _identifier;
	}
	else
		_transforms.at(_identifier)->ReadData(_sharedMemory._buffer, _circInfo.tail, _packageType);
}

void MDataHandler::LightHandler()
{
	if (_packageType == DESTROY)
	{
		delete _lights.at(_identifier);
		_lights.at(_identifier) = nullptr;
	}
	else if (_packageType == CREATE)
	{
		MLight* light = new MLight(_sharedMemory._buffer, _circInfo.tail);
		_lights.insert(pair<string, MLight*>(_identifier, light));
		_lights[_identifier]->GetData()->identifier = _identifier;
	}
	else
		_lights.at(_identifier)->ReadData(_sharedMemory._buffer, _circInfo.tail, _packageType);
}

void MDataHandler::MaterialHandler()
{
	if (_packageType == DESTROY)
	{
		delete _materials.at(_identifier);
		_materials.at(_identifier) = nullptr;
	}
	else if (_packageType == CREATE)
	{
		MMaterial* material = new MMaterial(_sharedMemory._buffer, _circInfo.tail);
		_materials.insert(pair<string, MMaterial*>(_identifier, material));
	}
	else
		_materials.at(_identifier)->ReadData(_sharedMemory._buffer, _circInfo.tail, _packageType);
}

void MDataHandler::CameraHandler()
{
	_camera.ReadData(_sharedMemory._buffer, _circInfo.tail, _packageType);
}

void MDataHandler::ShutDown()
{
	_meshes.clear();
	_lights.clear();
	_materials.clear();
	_transforms.clear();
}