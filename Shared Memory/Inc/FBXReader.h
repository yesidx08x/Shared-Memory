#pragma once
#include <fbxsdk.h>
#include <fbxsdk\fileio\fbxiosettings.h>
#include "DataStructures.h"

class FBXReader
{
public:
	FBXReader();
	FBXReader(string outpath, string inpath);
	~FBXReader();
	void ConvertFile(string name, GRFVersion version, bool standard = false);

private:
	FbxManager* _manager;
	FbxIOSettings* _ioSettings;
	FbxImporter* _importer;
	FbxScene* _scene;

	vector<VertexData> _meshData0;
	vector<SmallVertexData> _meshData1;
	unsigned int _vertexCount;

	int _numTabs = 0;

	bool _status;
	bool _standard;
	ofstream _outfile;

	string _outPath;
	string _outPathStandard;
	string _path;
	string _inPath;
	string _name;

	void PrintNode(FbxNode* node);
	void PrintAttribute(FbxNodeAttribute* attribute);
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
	void GetMesh0(FbxMesh* mesh);
	void GetMesh1(FbxMesh* mesh);
	//void GetElement(FbxMesh* mesh, fbxLayer* layer, int ctrlPointIndex, int vertexCounter, XMFLOAT3& output);

	void Check_Error(bool status);
};