#include "../Receiver/Inc/MCamera.h"

MCamera::MCamera()
{
	_data = new CameraData();
	_data->buffers = new CameraBuffers();
	XMStoreFloat4x4(&_data->buffers->projection, XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PI * 0.45f, 640.0f / 480.0f, 0.1f, 1000.0f)));
	XMStoreFloat4x4(&_data->buffers->view, XMMatrixTranspose(XMMatrixLookAtLH(XMVectorSet(0, 0, -5, 1), XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 0))));
}

MCamera::~MCamera()
{
	delete _data->buffers;
	delete _data;
}

CameraData* MCamera::GetData(){return _data;}

void MCamera::ReadData(void* buffer, unsigned int& tail, PackageType packageType)
{
	ReadWhole(buffer, tail);
}

void MCamera::ReadWhole(void* buffer, unsigned int& tail)
{
	memcpy(_data, (char*)buffer + tail, sizeof(CameraData));
	tail += sizeof(CameraData);

	_data->buffers->projection._11 = _data->buffers->projection._22 / (ScreenWidth / ScreenHeight);
}