#include "Camera.h"

Camera::Camera()
{
	_data.info = new CameraInfo();
	_data.buffers = new CameraBuffers();
}

Camera::~Camera()
{
	delete _data.info;
	delete _data.buffers;
}

void Camera::SetData(MObject& object, PackageType packageType)
{
	_identifierSize = 0;
	_packageType = packageType;
	_size = 0;

	float pm[4][4];
	M3dView view = M3dView::active3dView();
	MMatrix projectionM;
	view.projectionMatrix(projectionM);
	projectionM = projectionM.transpose();
	projectionM.get(pm);
	XMFLOAT4X4 projectionMatrix(
		pm[0][0], pm[0][1], pm[0][2], pm[0][3],
		pm[1][0], pm[1][1], pm[1][2], pm[1][3],
		pm[2][0], pm[2][1], pm[2][2], pm[2][3],
		pm[3][0], pm[3][1], pm[3][2], pm[3][3]);

	MMatrix viewMatrixM;
	view.modelViewMatrix(viewMatrixM);
	viewMatrixM = viewMatrixM.inverse().transpose();
	viewMatrixM.get(pm);
	XMFLOAT4X4 viewMatrix(
		pm[0][0], pm[0][1], pm[0][2], pm[0][3],
		pm[1][0], pm[1][1], pm[1][2], pm[1][3],
		pm[2][0], pm[2][1], pm[2][2], pm[2][3],
		pm[3][0], pm[3][1], pm[3][2], pm[3][3]);

	_data.info->position = XMFLOAT4(pm[0][3], pm[1][3], -pm[2][3], 1.0f);
	_data.buffers->projection = projectionMatrix;
	_data.buffers->view = viewMatrix;
	_size += sizeof(CameraBuffers);
	_size += sizeof(CameraInfo);
}

void Camera::SendPackage(void* buffer, unsigned int head)
{
	// Data
	memcpy((char*)buffer + head, _data.buffers, sizeof(CameraBuffers));
	head += sizeof(CameraBuffers);
	memcpy((char*)buffer + head, _data.info, sizeof(CameraInfo));
	head += sizeof(CameraInfo);
}