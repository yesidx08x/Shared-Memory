#include "FBXReader.h"

FBXReader::FBXReader(){}

FBXReader::FBXReader(string outpath, string inpath)
{
	_outPath = outpath;
	_outPathStandard = outpath + "Standards/";
	_inPath = inpath;
}

FBXReader::~FBXReader(){}

void FBXReader::ConvertFile(string name, GRFVersion version, bool standard)
{
	_standard = standard;
	_name = name + ".grf";

	if (_standard)
		_path = _outPathStandard + _name;
	else
		_path = _outPath + _name;
	// Initialize
	_manager = FbxManager::Create();
	_ioSettings = FbxIOSettings::Create(_manager, IOSROOT);
	_manager->SetIOSettings(_ioSettings);
	_importer = FbxImporter::Create(_manager, "");

	// Set file name
	string path;
	if(_standard)
		path = _outPathStandard + name + ".fbx";
	else
		path = _outPath + name + ".fbx";
	_status = _importer->Initialize(path.data(), -1, _manager->GetIOSettings());
	Check_Error(_status);

	// Import scene
	_scene = FbxScene::Create(_manager, "RootScene");
	_status = _importer->Import(_scene);
	Check_Error(_status);
	_importer->Destroy();

	// Print nodes
	FbxNode* rootNode = _scene->GetRootNode();
	if (rootNode)
	{
		for (int i = 0; i < rootNode->GetChildCount(); i++)
		{
			PrintNode(rootNode->GetChild(i));
			switch (version)
			{
			case GRFVersion::puntb:
				GetMesh0(rootNode->GetChild(i)->GetMesh());
				break;
			case GRFVersion::pu:
				GetMesh1(rootNode->GetChild(i)->GetMesh());
				break;
			default:
				break;
			}
		}
	}
	// Destroy the SDK manager and all the other objects it was handling.
	_manager->Destroy();
}

void FBXReader::PrintNode(FbxNode* node)
{
	for (int i = 0; i < _numTabs; i++)
		printf("\t");

	const char* nodeName = node->GetName();
	FbxDouble3 translation = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	FbxDouble3 scaling = node->LclScaling.Get();

	// Print the contents of the node.
	printf("node name = %s\ntranslation = (%f, %f, %f)\nrotation = (%f, %f, %f)\nscaling = (%f, %f, %f)\n",
		nodeName,
		translation[0], translation[1], translation[2],
		rotation[0], rotation[1], rotation[2],
		scaling[0], scaling[1], scaling[2]
	);
	_numTabs++;

	// Print the node's attributes.
	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
		PrintAttribute(node->GetNodeAttributeByIndex(i));

	// Recursively print the children.
	for (int j = 0; j < node->GetChildCount(); j++)
		PrintNode(node->GetChild(j));

	_numTabs--;
	for (int i = 0; i < _numTabs; i++)
		printf("\t");
	printf("\n");
}

void FBXReader::PrintAttribute(FbxNodeAttribute* attribute)
{
	if (!attribute) return;

	FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());
	FbxString attrName = attribute->GetName();
	//for (int i = 0; i < _numTabs; i++)
	//	printf("\t");
	// Note: to retrieve the character array of a FbxString, use its Buffer() method.
	printf("attribute type = \"%s\" name = \"%s\"\n", typeName.Buffer(), attrName.Buffer());
}

FbxString FBXReader::GetAttributeTypeName(FbxNodeAttribute::EType type)
{
	switch (type) {
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}

void FBXReader::GetMesh0(FbxMesh* mesh)
{
	FbxVector2 uvCoord;
	FbxVector4 normal;
	FbxStringList uvSetNames;
	//FbxGeometryElementNormal* elementNormal;
	FbxGeometryElementTangent* elementTanget;
	FbxGeometryElementBinormal* elementBitangent;
	bool unmapped;
	mesh->GetUVSetNames(uvSetNames);

	bool once = true;

	if (mesh != nullptr)
	{
		int polyCount = mesh->GetPolygonCount();
		for (int i = 0; i < polyCount; i++)
		{
			for (int j = 0; j < mesh->GetPolygonSize(i); j++)
			{
				// Vertices
				//mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j))[0];
				_meshData0.push_back(VertexData());

				//if (once)
				//{
				//	mesh->GetPolygonVertexNormal(i, j, normal);
				//	_meshData.back().vertex = XMFLOAT3((float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[0] + normal[0],
				//		(float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[1] + normal[1],
				//		(float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[2] * -1.0f + normal[2]);
				//	once = false;
				//}
				//else
				//{
				_meshData0.back().vertex = XMFLOAT3((float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[0],
						(float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[1],
						(float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[2]);
					//cout << "Vertex: " << _meshData.back().vertex.x << " " << _meshData.back().vertex.y << " " << _meshData.back().vertex.z << "\n";
				//}

				// UVs
				mesh->GetPolygonVertexUV(i, j, uvSetNames.GetStringAt(0), uvCoord, unmapped);
				if (!unmapped)
					_meshData0.back().uv = XMFLOAT2((float)uvCoord[0], 1.0f - (float)uvCoord[1]);

				// Normals
				mesh->GetPolygonVertexNormal(i, j, normal);
				_meshData0.back().normal = XMFLOAT3((float)normal[0], (float)normal[1], (float)normal[2]);
				//cout << "Normal: " << _meshData.back().normal.x << " "  << _meshData.back().normal.y << " " << _meshData.back().normal.z << "\n";
				//if (mesh->GetElementNormalCount() > 0)
				//{
				//	elementNormal = mesh->GetElementNormal(0);
				//	_meshData.back().normal.x = elementNormal->GetDirectArray().GetAt(_meshData.size() - 1)[0];
				//	_meshData.back().normal.y = elementNormal->GetDirectArray().GetAt(_meshData.size() - 1)[1];
				//	_meshData.back().normal.z = elementNormal->GetDirectArray().GetAt(_meshData.size() - 1)[2];
				//}

				// Tangents
				if (mesh->GetElementTangentCount() > 0)
				{
					elementTanget = mesh->GetElementTangent(0);
					_meshData0.back().tanget.x = (float)elementTanget->GetDirectArray().GetAt((int)_meshData0.size() - 1).mData[0];
					_meshData0.back().tanget.y = (float)elementTanget->GetDirectArray().GetAt((int)_meshData0.size() - 1).mData[1];
					_meshData0.back().tanget.z = (float)elementTanget->GetDirectArray().GetAt((int)_meshData0.size() - 1).mData[2];
				}

				// Bitangents
				if (mesh->GetElementBinormalCount() > 0)
				{
					elementBitangent = mesh->GetElementBinormal(0);
					_meshData0.back().bitanget.x = (float)elementBitangent->GetDirectArray().GetAt((int)_meshData0.size() - 1).mData[0];
					_meshData0.back().bitanget.y = (float)elementBitangent->GetDirectArray().GetAt((int)_meshData0.size() - 1).mData[1];
					_meshData0.back().bitanget.z = (float)elementBitangent->GetDirectArray().GetAt((int)_meshData0.size() - 1).mData[2];
				}
				
			}
		}
		_vertexCount = (unsigned int)_meshData0.size();

		// Write Mesh
		_outfile.open(_path, std::ofstream::binary);
		if (_outfile.is_open())
		{
			// Header
			_outfile.write((char*)&_vertexCount, sizeof(unsigned int));
			// Data
			_outfile.write((char*)_meshData0.data(), sizeof(VertexData) * _meshData0.size());

			PrintSuccess("Exported mesh \"" + _name + "\"");
		}
		else
			PrintError("Could not write to path \"" + _path + "\"");
		_outfile.close();
	}
}

void FBXReader::GetMesh1(FbxMesh* mesh)
{
	FbxVector2 uvCoord;
	FbxVector4 normal;
	FbxStringList uvSetNames;
	//FbxGeometryElementNormal* elementNormal;
	FbxGeometryElementTangent* elementTanget;
	FbxGeometryElementBinormal* elementBitangent;
	bool unmapped;
	mesh->GetUVSetNames(uvSetNames);

	bool once = true;

	if (mesh != nullptr)
	{
		int polyCount = mesh->GetPolygonCount();
		for (int i = 0; i < polyCount; i++)
		{
			for (int j = 0; j < mesh->GetPolygonSize(i); j++)
			{
				_meshData1.push_back(SmallVertexData());

				_meshData1.back().vertex = XMFLOAT3((float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[0],
					(float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[1],
					(float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[2]);

				// UVs
				mesh->GetPolygonVertexUV(i, j, uvSetNames.GetStringAt(0), uvCoord, unmapped);
				if (!unmapped)
					_meshData1.back().uv = XMFLOAT2((float)uvCoord[0], 1.0f - (float)uvCoord[1]);

			}
		}
		_vertexCount = (unsigned int)_meshData1.size();

		// Write Mesh
		_outfile.open(_path, std::ofstream::binary);
		if (_outfile.is_open())
		{
			// Header
			_outfile.write((char*)&_vertexCount, sizeof(unsigned int));
			// Data
			_outfile.write((char*)_meshData1.data(), sizeof(SmallVertexData) * _meshData1.size());

			PrintSuccess("Exported mesh \"" + _name + "\"");
		}
		else
			PrintError("Could not write to path \"" + _path + "\"");
		_outfile.close();
	}
}

//void FBXReader::GetElement(FbxMesh* mesh, int ctrlPointIndex, int vertexCounter, XMFLOAT3& output)
//{
//	switch (elementTanget->GetMappingMode())
//	{
//	case FbxGeometryElement::eByControlPoint:
//		break;
//	case FbxGeometryElement::eByPolygonVertex:
//		switch (elementTanget->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//			_meshData.back().tanget.x = elementTanget->GetDirectArray().GetAt(_meshData.size() - 1)[0];
//			_meshData.back().tanget.y = elementTanget->GetDirectArray().GetAt(_meshData.size() - 1)[1];
//			_meshData.back().tanget.z = elementTanget->GetDirectArray().GetAt(_meshData.size() - 1)[2];
//			break;
//		case FbxGeometryElement::eIndexToDirect:
//			break;
//		default:
//			break;
//		}
//		break;
//	default:
//		break;
//	}
//}

void FBXReader::Check_Error(bool status)
{
	if (!status)
	{
		HANDLE winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(winHandle, 12);
		cout << _importer->GetStatus().GetErrorString() << "\n";
		SetConsoleTextAttribute(winHandle, 15);
	}
}