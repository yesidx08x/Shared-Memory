#pragma once
#include "MObject.h"

class MSkinCluster : public MObject
{
public:
	MSkinCluster();
	MSkinCluster(void* buffer, unsigned int& tail);
	~MSkinCluster();

	void ReadData(void* buffer, unsigned int& tail, PackageType packageType = PACKAGETYPEAMOUNT);
	unsigned int GetJointAmount();
	vector<SkinClusterData>* GetData();
	//vector<XMFLOAT4X4> matrices;

private:
	SkinClusterHeader _header;
	vector<SkinClusterData> _data;
	unsigned int _jointAmount;

	void ReadWhole(void* buffer, unsigned int& tail);
};