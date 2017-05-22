#include "FBXConverter.h"

FBXConverter::FBXConverter(){}

FBXConverter::FBXConverter(string inpath, string exportpath)
{
	_inPath = inpath;
	_exportpath = exportpath;
}

FBXConverter::~FBXConverter()
{
	// Destroy the SDK manager and all the other objects it was handling.
	_manager != nullptr ? _manager->Destroy() : 0;
}

void FBXConverter::ConvertFile(string name, GRFVersion version)
{
	_name = name + ".grf";
	_exportpath += _name;

	// Initialize
	_manager = FbxManager::Create();
	_ioSettings = FbxIOSettings::Create(_manager, IOSROOT);
	_manager->SetIOSettings(_ioSettings);
	_importer = FbxImporter::Create(_manager, "");

	// Set file name
	_inPath = _inPath + name + ".fbx";

	_status = _importer->Initialize(_inPath.data(), -1, _manager->GetIOSettings());
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
}

void FBXConverter::GetMesh0(FbxMesh* mesh)
{
	FbxVector2 uvCoord;
	FbxVector4 normal;
	FbxStringList uvSetNames;
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
				_meshData0.push_back(VertexData());
				_meshData0.back().vertex = XMFLOAT3((float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[0],
						(float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[1],
						(float)mesh->GetControlPointAt(mesh->GetPolygonVertex(i, j)).mData[2]);

				// UVs
				mesh->GetPolygonVertexUV(i, j, uvSetNames.GetStringAt(0), uvCoord, unmapped);
				if (!unmapped)
					_meshData0.back().uv = XMFLOAT2((float)uvCoord[0], 1.0f - (float)uvCoord[1]);

				// Normals
				mesh->GetPolygonVertexNormal(i, j, normal);
				_meshData0.back().normal = XMFLOAT3((float)normal[0], (float)normal[1], (float)normal[2]);

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
		_outfile.open(_exportpath, std::ofstream::binary);
		if (_outfile.is_open())
		{
			// Header
			_outfile.write((char*)&_vertexCount, sizeof(unsigned int));
			// Data
			_outfile.write((char*)_meshData0.data(), sizeof(VertexData) * _meshData0.size());

			PrintSuccess("Exported mesh \"" + _name + "\"");
		}
		else
			PrintError("Could not write to path \"" + _exportpath + "\"");
		_outfile.close();
	}
}

void FBXConverter::GetMesh1(FbxMesh* mesh)
{
	FbxVector2 uvCoord;
	FbxVector4 normal;
	FbxStringList uvSetNames;
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
		_outfile.open(_exportpath, std::ofstream::binary);
		if (_outfile.is_open())
		{
			// Header
			_outfile.write((char*)&_vertexCount, sizeof(unsigned int));
			// Data
			_outfile.write((char*)_meshData1.data(), sizeof(SmallVertexData) * _meshData1.size());

			PrintSuccess("Exported mesh \"" + _name + "\"");
		}
		else
			PrintError("Could not write to path \"" + _exportpath + "\"");
		_outfile.close();
	}
}

void FBXConverter::Check_Error(bool status)
{
	if (!status)
	{
		HANDLE winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(winHandle, 12);
		cout << _importer->GetStatus().GetErrorString() << "\n";
		SetConsoleTextAttribute(winHandle, 15);
	}
}