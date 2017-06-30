#include "DataHandler.h"
/*
	This project does not fully work, there are some minor bugs that need to be found,
	these occurred when cleaning up the project.
*/

MCallbackIdArray DataHandler::_callbackIds;
string DataHandler::_path;
MStatus DataHandler::_status;
Mesh DataHandler::_mesh;
Camera DataHandler::_camera;
Transform DataHandler::_transform;
Light DataHandler::_light;
Material DataHandler::_material;
SharedMemory DataHandler::_sharedMemory;
SharedMemory::CircInfo DataHandler::_circInfo;
MessageHeader DataHandler::_messageHeader;
vector<DataHandler::ObjectEntiy> DataHandler::_objectEntities;
vector<DataHandler::ClientData*> DataHandler::_clientDatas;
bool DataHandler::_sendCamera;

DataHandler::DataHandler(){}

DataHandler::~DataHandler(){StopSharing();}

void DataHandler::StartSharing(unsigned int memorySize)
{
	_status = MS::kSuccess;
	Print(_sharedMemory.OpenMemory(memorySize, true));
	_sendCamera = false;

	//GetSceneData();
	_callbackIds.append(MDGMessage::addNodeAddedCallback(MeshCreationCB, "mesh", &_status));
	Check_Error(_status);
	_callbackIds.append(MDGMessage::addNodeAddedCallback(TransformCreationCB, "transform", &_status));
	Check_Error(_status);
	_callbackIds.append(MDGMessage::addNodeAddedCallback(MaterialCreationCB, "lambert", &_status));
	Check_Error(_status);
	_callbackIds.append(MDGMessage::addNodeAddedCallback(LightCreationCB, "light", &_status));
	Check_Error(_status);
	_callbackIds.append(MTimerMessage::addTimerCallback(1.0f, TimerFunctionCB, &_status));
	Check_Error(_status);

	MStringArray editorPanels;
	MGlobal::executeCommand("gpuCacheListModelEditorPanels", editorPanels);
	for (unsigned int i = 0; i < editorPanels.length(); i++)
	{
		_callbackIds.append(MUiMessage::add3dViewPostRenderMsgCallback(editorPanels[i], CameraUpdate, nullptr, &_status));
		Check_Error(_status, editorPanels[i]);
	}
}

void DataHandler::StopSharing()
{
	if (_sharedMemory.IsOpen())
	{
		SendPackage(SHUTDOWN, PACKAGETYPEAMOUNT, 0, 0);
		Print(_sharedMemory.CloseMemory());
		_status = MMessage::removeCallbacks(_callbackIds);
		Check_Error(_status);

		// Remove custom attribute
		MDGModifier modifier;
		MItDependencyNodes itDepnode;
		for (; !itDepnode.isDone(); itDepnode.next())
		{
			MFnDependencyNode depNode(itDepnode.item());
			MObject attrObject(depNode.attribute("SharedMemoryID", &_status));
			if (_status == MStatus::kSuccess)
			{
				_status = modifier.removeAttribute(depNode.object(), attrObject);
				Check_Error(_status);
				modifier.doIt();
			}
		}
	}
}

void DataHandler::SetChoices(Choices choices){_choices = choices;}

void DataHandler::SetPath(MString path){_path = path.asChar();}

void DataHandler::Remove(MDagPath & path)
{
	switch (path.apiType())
	{
	case MFn::kTransform:
	{
		_transform.SetData(path.node(), DESTROY);
		SendPackage(TRANSFORM, _transform.GetPackageType(), _transform.GetPackageSize(), _transform.GetIdentifierSize());
		break;
	}
	case MFn::kMesh:
		_mesh.SetData(path.node(), DESTROY);
		SendPackage(MESH, _mesh.GetPackageType(), _mesh.GetPackageSize(), _mesh.GetIdentifierSize());
		break;
	case MFn::kLight:
		Print(path.partialPathName());
		break;
	}
}

bool DataHandler::IsMemoryOpen()
{
	return _sharedMemory.IsOpen();
}

MStatus DataHandler::GetSceneData(MDagPath& dagPath)
{
	switch (dagPath.apiType())
	{
		case MFn::kTransform:
		{
			// Transforms, must be before mesh
			MFnTransform transform(dagPath);
			_clientDatas.push_back(new ClientData{ true });
			_transform.SetData(transform.object(), CREATE);
			SendPackage(TRANSFORM, _transform.GetPackageType(), _transform.GetPackageSize(), _transform.GetIdentifierSize());
			_callbackIds.append(MNodeMessage::addNodeDirtyCallback(transform.object(), TransformChangedCB, (void*)_clientDatas.back()));
			_callbackIds.append(MNodeMessage::addNodeAboutToDeleteCallback(transform.object(), NodeDestroyedCB, (void*)_clientDatas.back()));
			break;
		}
		case MFn::kDirectionalLight:
		{
			MFnLight light(dagPath);
			_clientDatas.push_back(new ClientData{ true });
			_light.SetData(light.object(), CREATE);
			SendPackage(LIGHT, _light.GetPackageType(), _light.GetPackageSize(), _light.GetIdentifierSize());
			_callbackIds.append(MNodeMessage::addNodeDirtyPlugCallback(light.object(), LightChangedCB, (void*)_clientDatas.back()));
			_callbackIds.append(MNodeMessage::addNodeAboutToDeleteCallback(light.object(), NodeDestroyedCB, (void*)_clientDatas.back()));
			break;
		}
		case MFn::kPointLight:
		{
			MFnLight light(dagPath);
			_clientDatas.push_back(new ClientData{ true });
			_light.SetData(light.object(), CREATE);
			SendPackage(LIGHT, _light.GetPackageType(), _light.GetPackageSize(), _light.GetIdentifierSize());
			_callbackIds.append(MNodeMessage::addNodeDirtyPlugCallback(light.object(), LightChangedCB, (void*)_clientDatas.back()));
			_callbackIds.append(MNodeMessage::addNodeAboutToDeleteCallback(light.object(), NodeDestroyedCB, (void*)_clientDatas.back()));
			break;
		}
		case MFn::kMesh:
		{
			MFnMesh mesh(dagPath);
			MObject plugObject = mesh.findPlug("inMesh", &_status).source().node();
			Check_Error(_status);
			MDagPath path;
			_status = mesh.getPath(path);
			Check_Error(_status);
			MFnDagNode dagNode(path);
			if (!dagNode.isIntermediateObject())
			{
				_clientDatas.push_back(new ClientData{ true });
				_mesh.SetData(mesh.object(), CREATE);
				SendPackage(MESH, _mesh.GetPackageType(), _mesh.GetPackageSize(), _mesh.GetIdentifierSize());

				for (unsigned int i = 0; i < mesh.parentCount(); i++)
				{
					if (mesh.parent(i).hasFn(MFn::kTransform))
					{
						_transform.SetData(mesh.parent(i), WHOLE);
						SendPackage(DataType::TRANSFORM, _transform.GetPackageType(), _transform.GetPackageSize(), _transform.GetIdentifierSize());
					}
				}
			}
			_callbackIds.append(MNodeMessage::addAttributeChangedCallback(mesh.object(), MeshChangedCB, (void*)_clientDatas.back()));
			_callbackIds.append(MNodeMessage::addNodeAboutToDeleteCallback(mesh.object(), NodeDestroyedCB, (void*)_clientDatas.back()));
			break;
		}
	}

	return MStatus::kSuccess;
}

void DataHandler::CameraUpdate(const MString &str, void *clientData)
{
	if (_sendCamera)
	{
		_camera.SetData(MObject(), WHOLE);
		SendPackage(CAMERA, _camera.GetPackageType(), _camera.GetPackageSize(), 0);
	}
}

void DataHandler::MeshCreationCB(MObject& object, void* clientData)
{
	// Add a callback specifik to every new mesh that are created
	_clientDatas.push_back(new ClientData{ false });
	_callbackIds.append(MNodeMessage::addAttributeChangedCallback(object, MeshChangedCB, (void*)_clientDatas.back()));
	_callbackIds.append(MNodeMessage::addNodeAboutToDeleteCallback(object, NodeDestroyedCB, (void*)_clientDatas.back()));
}

void DataHandler::MeshChangedCB(MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug, void* clientData)
{
	// check if the plug plug has in its name "doubleSided", which is an attribute that when is set we know that the mesh is finally available.
	// Only used for the creation of the mesh
	Print(plug.name());
	ClientData* data = (ClientData*)clientData;
	if (strstr(plug.name().asChar(), "doubleSided") != nullptr && MNodeMessage::AttributeMessage::kAttributeSet)
	{
		_mesh.SetData(plug.node(), CREATE);
		if (_mesh.GetPackageType() != NONE)
		{
			SendPackage(MESH, _mesh.GetPackageType(), _mesh.GetPackageSize(), _mesh.GetIdentifierSize());
			data->exist = true;
			memcpy((char*)clientData, data, sizeof(ClientData));

			MFnMesh mesh(plug);
			for (unsigned int i = 0; i < mesh.parentCount(); i++)
			{
				if (mesh.parent(i).hasFn(MFn::kTransform))
				{
					_transform.SetData(mesh.parent(i), WHOLE);
					SendPackage(DataType::TRANSFORM, _transform.GetPackageType(), _transform.GetPackageSize(), _transform.GetIdentifierSize());
				}
			}
		}
	}
	else if (strstr(plug.name().asChar(), "outMesh") && data->exist && !GetAsyncKeyState(VK_LBUTTON))
	{
		Print("UPDATE");
		_mesh.SetData(plug.node(), WHOLE);
		SendPackage(MESH, _mesh.GetPackageType(), _mesh.GetPackageSize(), _mesh.GetIdentifierSize());
	}
	else if (strstr(plug.name().asChar(), "instObjGroups") && data->exist) // Material
	{
		_mesh.SetData(plug.node(), MATERIALID);
		SendPackage(MESH, _mesh.GetPackageType(), _mesh.GetPackageSize(), _mesh.GetIdentifierSize());
	}
}

void DataHandler::TransformCreationCB(MObject& object, void * clientData)
{
	_clientDatas.push_back(new ClientData{ true });
	_transform.SetData(object, CREATE);
	SendPackage(TRANSFORM, _transform.GetPackageType(), _transform.GetPackageSize(), _transform.GetIdentifierSize());
	_callbackIds.append(MNodeMessage::addNodeDirtyPlugCallback(object, TransformChangedCB, (void*)_clientDatas.back()));
	_callbackIds.append(MNodeMessage::addNodeAboutToDeleteCallback(object, NodeDestroyedCB, (void*)_clientDatas.back()));
}

void DataHandler::TransformChangedCB(MObject& node, MPlug& plug, void* clientData)
{
	ClientData* data = (ClientData*)clientData;
	if (data->exist)
	{
		_transform.SetData(node, WHOLE);
		SendPackage(DataType::TRANSFORM, _transform.GetPackageType(), _transform.GetPackageSize(), _transform.GetIdentifierSize());

		MFnTransform transform(node, &_status);
		Check_Error(_status);
		for (unsigned int i = 0; i < transform.childCount(); i++)
		{
			if (transform.child(i).hasFn(MFn::kTransform))
			{
				_transform.SetData(transform.child(i), WHOLE);
				SendPackage(DataType::TRANSFORM, _transform.GetPackageType(), _transform.GetPackageSize(), _transform.GetIdentifierSize());
			}
		}
	}
}

void DataHandler::MaterialCreationCB(MObject& object, void* clientData)
{
	_clientDatas.push_back(new ClientData{ true });
	_material.SetData(object, CREATE);
	SendPackage(MATERIAL, _material.GetPackageType(), _material.GetPackageSize(), _material.GetIdentifierSize());
	_callbackIds.append(MNodeMessage::addNodeDirtyPlugCallback(object, MaterialChangedCB, (void*)_clientDatas.back()));
	_callbackIds.append(MNodeMessage::addNodeAboutToDeleteCallback(object, NodeDestroyedCB, (void*)_clientDatas.back()));

	MFnDependencyNode depNode(object, &_status);
	Check_Error(_status);
	MObject plugObject;
	MPlug plug = depNode.findPlug("color", &_status);
	Check_Error(_status);
	// Look for texture path if there is one
	MItDependencyGraph itGraph(plug, MFn::kFileTexture, MItDependencyGraph::kUpstream);
	MString texturePath;
	if (!itGraph.isDone())
	{
		_callbackIds.append(MNodeMessage::addAttributeChangedCallback(itGraph.thisNode(), FileNameChangeCB, (void*)_clientDatas.back(), &_status));
		Check_Error(_status);
	}
}

void DataHandler::MaterialChangedCB(MObject& node, MPlug& plug, void* clientData)
{
	ClientData* data = (ClientData*)clientData;
	if (data->exist)
	{
		_material.SetData(node, WHOLE);
		SendPackage(MATERIAL, _material.GetPackageType(), _material.GetPackageSize(), _material.GetIdentifierSize());
	}
}

void DataHandler::LightCreationCB(MObject& object, void* clientData)
{
	Print("LIGHTCALLBACK");
	_clientDatas.push_back(new ClientData{ true });
	_light.SetData(object, CREATE);
	SendPackage(LIGHT, _light.GetPackageType(), _light.GetPackageSize(), _light.GetIdentifierSize());
	_callbackIds.append(MNodeMessage::addNodeDirtyPlugCallback(object, LightChangedCB, (void*)_clientDatas.back()));
	_callbackIds.append(MNodeMessage::addNodeAboutToDeleteCallback(object, NodeDestroyedCB, (void*)_clientDatas.back()));
}

void DataHandler::LightChangedCB(MObject& node, MPlug& plug, void* clientData)
{
	ClientData* data = (ClientData*)clientData;
	if (data->exist)
	{
		_light.SetData(node, WHOLE);
		SendPackage(LIGHT, _light.GetPackageType(), _light.GetPackageSize(), _light.GetIdentifierSize());
	}
}

void DataHandler::FileNameChangeCB(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void *clientData)
{
	ClientData* data = (ClientData*)clientData;
	if (strstr(plug.name().asChar(), "fileTextureName") && data->exist)
	{
		_material.SetData(plug.node(), TEXNAME);
		SendPackage(MATERIAL, _material.GetPackageType(), _material.GetPackageSize(), _material.GetIdentifierSize());
	}
}

void DataHandler::NodeDestroyedCB(MObject& object, MDGModifier& modifier, void* clientData)
{
	Print("DESTROYED");
	ClientData* data = (ClientData*)clientData;
	switch (object.apiType())
	{
	case MFn::kMesh:
		//SendPackage(MESH, DESTROY, 0,  );
		break;
	default:
		break;
	}
	data->exist = false;
	memcpy((char*)clientData, data, sizeof(ClientData));
}

void DataHandler::TimerFunctionCB(float elapsedTime, float lastTime, void *clientData)
{
	if (_sharedMemory.AddedReader())
	{
		Print("Added Reader!");
	}
}

void DataHandler::SendPackage(DataType type, PackageType packageType, unsigned int packageSize, unsigned int identifierSize)
{
	do
	{
		_circInfo = _sharedMemory.GetInfo();

		if ((packageSize + sizeof(MessageHeader)) <= _circInfo.freeMem && packageType != NONE)
		{
			// Reset header if at end of buffer
			if (packageSize > _circInfo.freeMem)
			{
				_circInfo.head = sizeof(SharedMemory::CircInfo);
			}
			// Send header
			_messageHeader.type = type;
			_messageHeader.subType = packageType;
			_messageHeader.size = packageSize;
			_messageHeader.identifierSize = identifierSize;
			memcpy((char*)_sharedMemory._buffer + _circInfo.head, &_messageHeader, sizeof(MessageHeader));
			_circInfo.head += sizeof(MessageHeader);

			// Send main data
			switch (type)
			{
			case CAMERA:
				_camera.SendPackage(_sharedMemory._buffer, _circInfo.head);
				break;
			case TRANSFORM:
				_transform.SendPackage(_sharedMemory._buffer, _circInfo.head);
				break;
			case MESH:
				_mesh.SendPackage(_sharedMemory._buffer, _circInfo.head);
				break;
			case LIGHT:
				_light.SendPackage(_sharedMemory._buffer, _circInfo.head);
				break;
			case MATERIAL:
				_material.SendPackage(_sharedMemory._buffer, _circInfo.head);
				break;
			case SHUTDOWN:
			default:
				break;
			}

			// Move header
			_circInfo.freeMem -= (packageSize + sizeof(MessageHeader) + identifierSize);
			_circInfo.head += packageSize + identifierSize;
			_circInfo.SentCount++;
			_sharedMemory.SetInfo(_circInfo);
			break;
		}
	} while (_sharedMemory.GetInfo().freeMem > !packageSize);
}

void DataHandler::ExportSelected()
{
	MSelectionList activeList;
	MGlobal::getActiveSelectionList(activeList);
	MItSelectionList itSelection(activeList);
	for (; !itSelection.isDone(); itSelection.next())
	{
		MDagPath item;
		MObject component;
		itSelection.getDagPath(item, component);
		if (item.hasFn(MFn::kMesh))
			ExportMesh(item);
	}
}

void DataHandler::SendCamera(bool send){_sendCamera = send;}

void DataHandler::ExportMesh(MDagPath& dagPath)
{
	MFnMesh mesh(dagPath);

	float2 uv;
	MVector normal;
	MVector tangent;
	MPointArray points;
	MIntArray vertexList;

	vector<VertexData> data;
	MItMeshPolygon itPoly(mesh.object());
	while (!itPoly.isDone())
	{
		itPoly.getTriangles(points, vertexList);

		for (int i = (points.length() - 1); i >= 0; i--)
		{
			itPoly.getUVAtPoint(points[i], uv);
			mesh.getFaceVertexNormal(itPoly.index(), vertexList[i], normal);
			mesh.getFaceVertexTangent(itPoly.index(), vertexList[i], tangent);

			data.push_back(VertexData());
			data.back().vertex = XMFLOAT3(points[i].x, points[i].y, -points[i].z);
			data.back().uv = XMFLOAT2(uv[0], 1.0f - uv[1]);
			data.back().normal = XMFLOAT3(normal.x, normal.y, -normal.z);
			data.back().tanget = XMFLOAT3(tangent.x, tangent.y, -tangent.z);
		}
		itPoly.next();
	}

	ofstream outfile;
	string name = mesh.name().asChar();
	outfile.open(_path + name + ".grf", std::ofstream::binary);
	if (outfile.is_open())
	{
		// Header
		unsigned int size = data.size();
		outfile.write((char*)&size, sizeof(unsigned int));
		// Data
		outfile.write((char*)data.data(), sizeof(VertexData) * size);
		Print("Exported " + mesh.name());
	}
	else
		Print("Error while exporting " + mesh.name());
	outfile.close();
}