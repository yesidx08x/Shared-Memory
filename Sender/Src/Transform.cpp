#include "Transform.h"

Transform::Transform(){}

Transform::~Transform(){}

void Transform::SetData(MObject& object, PackageType packageType)
{
	_identifierSize = 0;
	_size = 0;
	_packageType = packageType;

	MFnTransform transform(object);
	SetIdentifier(transform.name());

	if (packageType != DESTROY)
	{
		MVector position;
		double scale[3];
		double rotation[3];
		MTransformationMatrix::RotationOrder order;
		position = transform.getTranslation(MSpace::kTransform);
		transform.getScale(scale);
		transform.getRotation(rotation, order);

		_data.transform.position = XMFLOAT4(position.x, position.y, -position.z, 1.0f);
		_data.transform.scale = XMFLOAT4(scale[0], scale[1], scale[2], 1.0f);
		XMStoreFloat4x4(&_data.transform.rotation, XMMatrixIdentity());

		XMMATRIX rotX = XMMatrixRotationNormal(XMVectorSet(-1.0, 0.0f, 0.0f, 0.0f), rotation[0]);
		XMMATRIX rotY = XMMatrixRotationNormal(XMVectorSet(0.0, -1.0f, 0.0f, 0.0f), rotation[1]);
		XMMATRIX rotZ = XMMatrixRotationNormal(XMVectorSet(0.0, 0.0f, -1.0f, 0.0f), rotation[2]);
		XMStoreFloat4x4(&_data.transform.rotation, rotX * rotY * rotZ);

		_size += sizeof(MatrixData);
	}
}

void Transform::SendPackage(void * buffer, unsigned int head)
{
	if (_packageType != DESTROY)
	{
		// Identifier
		memcpy((char*)buffer + head, _identifier.data(), _identifierSize);
		head += _identifierSize;
		// Data
		memcpy((char*)buffer + head, &_data.transform, sizeof(MatrixData));
		head += sizeof(MatrixData);
	}
	else
		SendIdentifier(buffer, head);
}