#include "Entity/Transform.h"
#include "Entity/EntityHandler.h"

Transform::Transform(){}

Transform::~Transform(){}

void Transform::BindTransform(Entity & entity, string identifier, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotationVector, float degrees)
{
	for (unsigned int i = 0; i < _transforms.size(); i++)
	{
		if (identifier == _transforms[i].identifier && _transforms[i].nrOfUsers > 0)
		{
			entity.transformID = i;
			_transforms[i].nrOfUsers++;
			return;
		}
	}
	_transforms.push_back(TransformData());
	entity.transformID = (int)_transforms.size() - 1;
	_transforms[entity.transformID].identifier = identifier;
	_transforms[entity.transformID].nrOfUsers = 1;

	_transforms[entity.transformID].position = XMFLOAT4(position.x, position.y, position.z, 1.0f);
	_transforms[entity.transformID].scale = XMFLOAT4(scale.x, scale.y, scale.z, 1.0f);

	XMVECTOR vector = XMLoadFloat3(& rotationVector);
	XMMATRIX rotMatrix = XMMatrixRotationNormal(vector, XMConvertToRadians(degrees));
	XMStoreFloat4x4(&_transforms[entity.transformID].rotation, rotMatrix);

	XMStoreFloat4(&_transforms[entity.transformID].direction, XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotMatrix));
	XMStoreFloat4(&_transforms[entity.transformID].right, XMVector4Transform(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotMatrix));
	XMStoreFloat4(&_transforms[entity.transformID].up, XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotMatrix));
}

void Transform::BindTransform(Entity & entity, TransformData & transformData)
{
	for (unsigned int i = 0; i < _transforms.size(); i++)
	{
		if (transformData.identifier == _transforms[i].identifier && _transforms[i].nrOfUsers > 0)
		{
			entity.transformID = i;
			_transforms[i].nrOfUsers++;
			return;
		}
	}
	_transforms.push_back(TransformData());
	entity.transformID = (int)_transforms.size() - 1;
	_transforms[entity.transformID].identifier = transformData.identifier;
	_transforms[entity.transformID].nrOfUsers = 1;

	_transforms[entity.transformID].direction = transformData.direction;
	_transforms[entity.transformID].parent = transformData.parent;
	_transforms[entity.transformID].position = transformData.position;
	_transforms[entity.transformID].right = transformData.right;
	_transforms[entity.transformID].rotation = transformData.rotation;
	_transforms[entity.transformID].scale = transformData.scale;
	_transforms[entity.transformID].up = transformData.up;
}

void Transform::BindChild(Entity & parent, Entity & child)
{
	_transforms[child.transformID].parent = parent.transformID;
}

void Transform::RemoveTransform(Entity & entity)
{
	if (entity.transformID >= 0)
	{
		_transforms[entity.transformID].nrOfUsers--;
		if (_transforms[entity.transformID].nrOfUsers <= 0)
		{
			_transforms[entity.transformID].direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			_transforms[entity.transformID].position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			_transforms[entity.transformID].scale = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			_transforms[entity.transformID].right = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
			_transforms[entity.transformID].up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
			XMStoreFloat4x4(&_transforms[entity.transformID].rotation, XMMatrixIdentity());
			_transforms[entity.transformID].transform.position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			_transforms[entity.transformID].transform.scale = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			XMStoreFloat4x4(&_transforms[entity.transformID].transform.rotation, XMMatrixIdentity());
			_transforms[entity.transformID].nrOfUsers = 0;
			_transforms[entity.transformID].identifier = "";
		}
		entity.meshID = -1;
	}
}

void Transform::RemoveParent(Entity & entity)
{
	_transforms[entity.transformID].parent = -1;
}

// Translation
void Transform::SetPosition(Entity & entity, XMFLOAT4 position)
{
	_transforms[entity.transformID].position = position;
}

XMFLOAT4 Transform::GetPosition(Entity & entity){return _transforms[entity.transformID].position;}

void Transform::MoveForward(Entity & entity, float value)
{
	XMVECTOR position = XMLoadFloat4(&_transforms[entity.transformID].position);
	XMVECTOR direction = XMLoadFloat4(&_transforms[entity.transformID].direction);
	position += direction * value;
	XMStoreFloat4(&_transforms[entity.transformID].position, position);
}

void Transform::MoveBackward(Entity & entity, float value)
{
	XMVECTOR position = XMLoadFloat4(&_transforms[entity.transformID].position);
	XMVECTOR direction = XMLoadFloat4(&_transforms[entity.transformID].direction);
	position -= direction * value;
	XMStoreFloat4(&_transforms[entity.transformID].position, position);
}

void Transform::MoveLeft(Entity & entity, float value)
{
	XMVECTOR position = XMLoadFloat4(&_transforms[entity.transformID].position);
	XMVECTOR right = XMLoadFloat4(&_transforms[entity.transformID].right);
	position -= right * value;
	XMStoreFloat4(&_transforms[entity.transformID].position, position);
}

void Transform::MoveRight(Entity & entity, float value)
{
	XMVECTOR position = XMLoadFloat4(&_transforms[entity.transformID].position);
	XMVECTOR right = XMLoadFloat4(&_transforms[entity.transformID].right);
	position += right * value;
	XMStoreFloat4(&_transforms[entity.transformID].position, position);
}

void Transform::MoveUp(Entity & entity, float value)
{
	XMVECTOR position = XMLoadFloat4(&_transforms[entity.transformID].position);
	XMVECTOR up = XMLoadFloat4(&_transforms[entity.transformID].up);
	position += up * value;
	XMStoreFloat4(&_transforms[entity.transformID].position, position);
}

void Transform::MoveDown(Entity & entity, float value)
{
	XMVECTOR position = XMLoadFloat4(&_transforms[entity.transformID].position);
	XMVECTOR up = XMLoadFloat4(&_transforms[entity.transformID].up);
	position -= up * value;
	XMStoreFloat4(&_transforms[entity.transformID].position, position);
}

// Rotation
void Transform::RotatePitch(Entity & entity, float radians)
{
	XMMATRIX rotation = XMLoadFloat4x4(&_transforms[entity.transformID].rotation);
	XMVECTOR direction = XMLoadFloat4(&_transforms[entity.transformID].direction);
	XMVECTOR right = XMLoadFloat4(&_transforms[entity.transformID].right);
	XMVECTOR up = XMLoadFloat4(&_transforms[entity.transformID].up);

	rotation *= XMMatrixRotationAxis(right, radians);
	direction = XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotation);
	up = XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotation);

	XMStoreFloat4(&_transforms[entity.transformID].direction, direction);
	XMStoreFloat4(&_transforms[entity.transformID].up, up);
	XMStoreFloat4x4(&_transforms[entity.transformID].rotation, rotation);
}

void Transform::RotateYaw(Entity & entity, float radians)
{
	XMMATRIX rotation = XMLoadFloat4x4(&_transforms[entity.transformID].rotation);
	XMVECTOR direction = XMLoadFloat4(&_transforms[entity.transformID].direction);
	XMVECTOR right = XMLoadFloat4(&_transforms[entity.transformID].right);
	XMVECTOR up = XMLoadFloat4(&_transforms[entity.transformID].up);

	rotation *= XMMatrixRotationAxis(up, radians);
	direction = XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotation);
	right = XMVector4Transform(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotation);

	XMStoreFloat4(&_transforms[entity.transformID].direction, direction);
	XMStoreFloat4(&_transforms[entity.transformID].right, right);
	XMStoreFloat4x4(&_transforms[entity.transformID].rotation, rotation);
}

void Transform::RotateRoll(Entity & entity, float radians)
{
	XMMATRIX rotation = XMLoadFloat4x4(&_transforms[entity.transformID].rotation);
	XMVECTOR direction = XMLoadFloat4(&_transforms[entity.transformID].direction);
	XMVECTOR right = XMLoadFloat4(&_transforms[entity.transformID].right);
	XMVECTOR up = XMLoadFloat4(&_transforms[entity.transformID].up);

	rotation *= XMMatrixRotationAxis(direction, radians);
	right = XMVector4Transform(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotation);
	up = XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotation);

	XMStoreFloat4(&_transforms[entity.transformID].direction, up);
	XMStoreFloat4(&_transforms[entity.transformID].right, right);
	XMStoreFloat4x4(&_transforms[entity.transformID].rotation, rotation);
}

void Transform::SetRotation(Entity& entity, XMVECTOR vector, float degrees)
{
	XMMATRIX rotMatrix = XMMatrixRotationNormal(vector, XMConvertToRadians(degrees));
	XMStoreFloat4x4(&_transforms[entity.transformID].rotation, rotMatrix);

	XMStoreFloat4(&_transforms[entity.transformID].direction, XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotMatrix));
	XMStoreFloat4(&_transforms[entity.transformID].right, XMVector4Transform(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotMatrix));
	XMStoreFloat4(&_transforms[entity.transformID].up, XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotMatrix));
}

void Transform::SetRotation(Entity & entity, XMFLOAT4X4 matrix)
{
	XMMATRIX rotMatrix = XMLoadFloat4x4(&matrix);
	_transforms[entity.transformID].rotation = matrix;

	XMStoreFloat4(&_transforms[entity.transformID].direction, XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotMatrix));
	XMStoreFloat4(&_transforms[entity.transformID].right, XMVector4Transform(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotMatrix));
	XMStoreFloat4(&_transforms[entity.transformID].up, XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotMatrix));
}

// Scale
void Transform::SetScale(Entity & entity, XMFLOAT4 scale)
{
	_transforms[entity.transformID].scale = scale;
}

void Transform::SetPosScaleRot(Entity & entity, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotationVector, float degrees)
{
	_transforms[entity.transformID].position = XMFLOAT4(position.x, position.y, position.z, 1.0f);
	_transforms[entity.transformID].scale = XMFLOAT4(scale.x, scale.y, scale.z, 1.0f);

	XMVECTOR vector = XMLoadFloat3(&rotationVector);
	XMMATRIX rotMatrix = XMMatrixRotationNormal(vector, XMConvertToRadians(degrees));
	XMStoreFloat4x4(&_transforms[entity.transformID].rotation, rotMatrix);

	XMStoreFloat4(&_transforms[entity.transformID].direction, XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotMatrix));
	XMStoreFloat4(&_transforms[entity.transformID].right, XMVector4Transform(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotMatrix));
	XMStoreFloat4(&_transforms[entity.transformID].up, XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotMatrix));
}

int Transform::CreateTransform()
{
	TransformData transformData;
	_transforms.push_back(transformData);
	return (int)_transforms.size() - 1;
}

XMFLOAT4X4 Transform::GetEntityTransform(Entity & entity)
{
	XMVECTOR position = XMLoadFloat4(&_transforms[entity.transformID].position);
	XMMATRIX rotation = XMLoadFloat4x4(&_transforms[entity.transformID].rotation);
	XMVECTOR scale = XMLoadFloat4(&_transforms[entity.transformID].scale);
	XMMATRIX matrix = rotation * XMMatrixScalingFromVector(scale) * XMMatrixTranslationFromVector(position);
	XMFLOAT4X4 returnMatrix;
	XMStoreFloat4x4(&returnMatrix, matrix);
	return returnMatrix;
}