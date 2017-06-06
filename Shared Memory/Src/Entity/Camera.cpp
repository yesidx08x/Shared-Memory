#include "Entity/Camera.h"
#include "Entity/EntityHandler.h"
#include "Entity/Transform.h"

Camera::Camera() {}

Camera::Camera(Transform* transform)
{
	_transform = transform;
}

Camera::~Camera() 
{
	for (size_t i = 0; i < _cameras.size(); i++)
	{
		_cameras[i].buffers == nullptr ? 0 : _aligned_free(_cameras[i].buffers);
		_cameras[i].info == nullptr ? 0 : _aligned_free(_cameras[i].info);
	}
}

void Camera::BindCamera(Entity& entity, float fov, float aspectRatio, float nearPlane, float farPlane, bool setActive, float moveSpeed, float rotationSpeed)
{
	_cameras.push_back(CameraData());
	_cameras.back().buffers = (CameraBuffers*)_aligned_malloc(sizeof(CameraBuffers), 16);
	_cameras.back().info = (CameraInfo*)_aligned_malloc(sizeof(CameraInfo), 16);
	if (setActive)
	{
		_activeCamera.cameraID = (int)_cameras.size() - 1;
		_activeCamera.entityID = entity.identifier;
	}
	entity.cameraID = (int)_cameras.size() - 1;
	_cameras[_cameras.size() - 1].transformID = entity.transformID;

	XMFLOAT4 position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 focusPoint = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	XMFLOAT4 upVector = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 offset = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	_moveSpeed = moveSpeed;
	_rotationSpeed = rotationSpeed;

	_cameras.back().info->position = position;
	_cameras.back().info->focusPoint = focusPoint;
	_cameras.back().upVector = upVector;
	_cameras.back().offset = offset;

	XMStoreFloat4x4(&_cameras.back().buffers->projection, XMMatrixTranspose(XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane)));
	XMStoreFloat4x4(&_cameras.back().buffers->view, XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(position.x, position.y, position.z, position.w),
		XMVectorSet(focusPoint.x, focusPoint.y, focusPoint.z, focusPoint.w),
		XMVectorSet(upVector.x, upVector.y, upVector.z, upVector.w))));
}

CameraData* Camera::GetActiveCameraData() { return &_cameras[_activeCamera.cameraID]; }

string Camera::GetActiveEntityID(){return _activeCamera.entityID;}

void Camera::UpdateActiveCamera(Entity& entity, float dt)
{
	if (!_shared)
	{
		GetKeayInput(entity, dt);
		_activeData = &_transform->_transforms[_cameras[_activeCamera.cameraID].transformID];
		XMVECTOR offset = DirectX::XMVectorSet(_cameras[_activeCamera.cameraID].offset.x, _cameras[_activeCamera.cameraID].offset.y, _cameras[_activeCamera.cameraID].offset.z, 0.0f);
		XMVECTOR position = XMLoadFloat4(&_activeData->position);
		XMVECTOR up = XMLoadFloat4(&_activeData->up);
		XMVECTOR focusPoint = XMLoadFloat4(&_activeData->direction);
		focusPoint = position + offset + focusPoint;

		XMStoreFloat4x4(&_cameras[_activeCamera.cameraID].buffers->view, XMMatrixTranspose(XMMatrixLookAtLH(position, focusPoint, up)));
		XMStoreFloat4(&_cameras[_activeCamera.cameraID].info->position, position);
		XMStoreFloat4(&_cameras[_activeCamera.cameraID].info->focusPoint, position - focusPoint);
	}
}

void Camera::UpdateActiveCamera(CameraData& data)
{
	//_cameras[_activeCamera].buffers.projection = data.buffers.projection;
	_cameras[_activeCamera.cameraID].buffers->view = data.buffers->view;
	_cameras[_activeCamera.cameraID].info->position = data.info->position;
}

void Camera::UseSharedCamera(bool shared){_shared = shared;}

void Camera::GetKeayInput(Entity& entity, float dt)
{
	if (GetAsyncKeyState(87)) // W
		_transform->RotatePitch(entity, -dt * _rotationSpeed);
	if (GetAsyncKeyState(83)) // S
		_transform->RotatePitch(entity, dt * _rotationSpeed);
	if (GetAsyncKeyState(65)) // A
		_transform->RotateYaw(entity, -dt * _rotationSpeed);
	if (GetAsyncKeyState(68)) // D
		_transform->RotateYaw(entity, dt * _rotationSpeed);
	if (GetAsyncKeyState(69)) // D
		_transform->MoveUp(entity, dt * _moveSpeed);
	if (GetAsyncKeyState(81)) // D
		_transform->MoveDown(entity, dt * _moveSpeed);
	if (GetAsyncKeyState(VK_UP))
		_transform->MoveForward(entity, dt * _moveSpeed);
	if (GetAsyncKeyState(VK_DOWN))
		_transform->MoveBackward(entity, dt * _moveSpeed);
	if (GetAsyncKeyState(VK_RIGHT))
		_transform->MoveRight(entity, dt * _moveSpeed);
	if (GetAsyncKeyState(VK_LEFT))
		_transform->MoveLeft(entity, dt * _moveSpeed);
}