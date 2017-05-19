#pragma once
#include <fbxsdk.h>
#include <fbxsdk\fileio\fbxiosettings.h>
#include "DataStructures.h"

class FBXConverter
{
public:
	FBXConverter();
	FBXConverter(string outpath, string inpath);
	~FBXConverter();
	void ConvertFile(string name, GRFVersion version);

private:
	FbxManager* _manager;
	FbxIOSettings* _ioSettings;
	FbxImporter* _importer;
	FbxScene* _scene;

	vector<VertexData> _meshData0;
	vector<SmallVertexData> _meshData1;
	unsigned int _vertexCount;

	bool _status;
	ofstream _outfile;

	string _exportpath;
	string _inPath;
	string _name;

	void GetMesh0(FbxMesh* mesh);
	void GetMesh1(FbxMesh* mesh);

	void Check_Error(bool status);
};