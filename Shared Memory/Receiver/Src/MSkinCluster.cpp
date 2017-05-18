#include "../Receiver/Inc/MSkinCluster.h"

MSkinCluster::MSkinCluster() {}

MSkinCluster::MSkinCluster(void* buffer, unsigned int& tail)
{
	ReadWhole(buffer, tail);
}

MSkinCluster::~MSkinCluster() {}

vector<SkinClusterData>* MSkinCluster::GetData() { return &_data; }

unsigned int MSkinCluster::GetJointAmount(){	return _jointAmount;}

void MSkinCluster::ReadData(void* buffer, unsigned int& tail, PackageType packageType)
{
	switch (packageType)
	{
	case WHOLE:
		ReadWhole(buffer, tail);
		break;
	case CREATE:
		ReadWhole(buffer, tail);
		break;
	default:
		break;
	}
}

void MSkinCluster::ReadWhole(void* buffer, unsigned int& tail)
{
	// Header
	memcpy(&_header, (char*)buffer + tail, sizeof(SkinClusterHeader));
	tail += sizeof(SkinClusterHeader);
	_jointAmount = _header.jointAmount;
	_data.resize(_jointAmount);

	// Data
	memcpy(_data.data(), (char*)buffer + tail, sizeof(SkinClusterData) * _jointAmount);
	tail += sizeof(SkinClusterData) * _jointAmount;

	//vector<XMFLOAT4X4> toRootTransforms;
	//toRootTransforms.resize(_jointAmount);
	//toRootTransforms[0] = _data[0].transform;
	//for (unsigned int i = 1; i < _jointAmount; i++)
	//{
	//	// Current bone transform relative to its parent
	//	XMMATRIX toParent = XMLoadFloat4x4(&_data[i].transform);
	//																
	//	XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[i-1]);
	//	XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);
	//	XMStoreFloat4x4(&toRootTransforms[i], toRoot);
	//}

	//matrices.resize(_jointAmount);
	//for (int i = 0; i < _jointAmount; i++)
	//{
	//	XMMATRIX offset = XMLoadFloat4x4(&_data[i].inverseBind);
	//	XMMATRIX toRoot = XMLoadFloat4x4(&toRootTransforms[i]);
	//	XMStoreFloat4x4(&matrices[i], XMMatrixMultiply(offset, toRoot));
	//}
}