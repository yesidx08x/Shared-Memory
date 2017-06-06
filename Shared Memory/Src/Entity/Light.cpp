#include "Entity/Light.h"
#include "Entity/EntityHandler.h"
#include "Entity/Transform.h"

Light::Light(){}

Light::Light(Transform * transform)
{
	_transform = transform;
}

Light::~Light()
{
	for (size_t i = 0; i < _lightData.size(); i++)
	{
		_lightData[i].buffer == nullptr ? 0 : _aligned_free(_lightData[i].buffer);
	}
}

void Light::BindLight(Entity& entity, string name, LightType type)
{
	for (unsigned int i = 0; i < _lightData.size(); i++)
	{
		if (name == _lightData[i].identifier && _lightData[i].nrOfUsers > 0) // Look for an already loaded light
		{
			entity.lightID = i;
			_lightData[i].nrOfUsers++;
			return;
		}
	}
	_lightData.push_back(LightData());
	_lightData.back().buffer = (LightBuffer*)_aligned_malloc(sizeof(LightBuffer), 16);;
	_lightData.back().buffer->active = 1;
	entity.lightID = (int)_lightData.size() - 1;
	if (type == point)
	{
		entity.shaderID = 1;
		entity.materialsIDs.albedo = 8;
		entity.meshID = 0;
	}
	else if (type == directional)
	{
		entity.shaderID = 2;
		entity.materialsIDs.albedo = 9;
		entity.meshID = 1;
	}

	_lightData[entity.lightID].identifier = name;
	_lightData[entity.lightID].type = type;
	_lightData[entity.lightID].nrOfUsers = 1;
	_lightData[entity.lightID].update = false;

	if (entity.transformID > -1)
	{
		_transform->_transforms[entity.transformID].direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR vDirection = XMVector4Normalize(XMLoadFloat4(&XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f)));
		XMMATRIX rotMatrix = XMMatrixInverse(NULL, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), vDirection, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		XMStoreFloat4x4(&_transform->_transforms[entity.transformID].rotation, rotMatrix);

		XMStoreFloat4(&_transform->_transforms[entity.transformID].direction, XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotMatrix));
		XMStoreFloat4(&_transform->_transforms[entity.transformID].up, XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotMatrix));
		XMStoreFloat4(&_transform->_transforms[entity.transformID].right, XMVector4Transform(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotMatrix));
	}
}

void Light::BindLight(Entity & entity, LightData data)
{
	for (unsigned int i = 0; i < _lightData.size(); i++)
	{
		if (data.identifier == _lightData[i].identifier && _lightData[i].nrOfUsers > 0) // Look for an already loaded mesh
		{
			entity.lightID = i;
			_lightData[i].nrOfUsers++;
			return;
		}
	}
	_lightData.push_back(LightData());
	_lightData.back().buffer->active = 1;
	entity.lightID = (int)_lightData.size() - 1;
	if (data.type == point)
	{
		entity.shaderID = 1;
		entity.materialsIDs.albedo = 8;
		entity.meshID = 0;
	}
	else if (data.type == directional)
	{
		entity.shaderID = 2;
		entity.materialsIDs.albedo = 9;
		entity.meshID = 1;
	}

	_lightData[entity.lightID].identifier = data.identifier;
	_lightData[entity.lightID].type = data.type;
	_lightData[entity.lightID].nrOfUsers = 1;
	_lightData[entity.lightID].update = false;

	if (entity.transformID > -1)
	{
		_transform->_transforms[entity.transformID].direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR vDirection = XMVector4Normalize(XMLoadFloat4(&XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f)));
		XMMATRIX rotMatrix = XMMatrixInverse(NULL, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), vDirection, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		XMStoreFloat4x4(&_transform->_transforms[entity.transformID].rotation, rotMatrix);

		XMStoreFloat4(&_transform->_transforms[entity.transformID].direction, XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotMatrix));
		XMStoreFloat4(&_transform->_transforms[entity.transformID].up, XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotMatrix));
		XMStoreFloat4(&_transform->_transforms[entity.transformID].right, XMVector4Transform(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotMatrix));
	}
}

void Light::RemoveLight(Entity & entity)
{
	if (entity.lightID >= 0)
	{
		_lightData[entity.lightID].nrOfUsers--;
		if (_lightData[entity.lightID].nrOfUsers <= 0)
		{
			_lightData[entity.lightID].buffer->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			_lightData[entity.lightID].buffer->direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
			_lightData[entity.lightID].buffer->intesity = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			_lightData[entity.lightID].buffer->position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
			_lightData[entity.lightID].buffer->active = 0;
			_lightData[entity.lightID].nrOfUsers = 0;
			_lightData[entity.lightID].identifier = "";
			_lightData[entity.lightID].update = false;
		}
		entity.lightID = -1;
	}
}

void Light::SetColor(Entity & entity, XMFLOAT4 & color)
{
	_lightData[entity.lightID].buffer->color = color;
	_lightData[entity.lightID].update = true;
}

void Light::SetIntensity(Entity & entity, float value)
{
	_lightData[entity.lightID].buffer->intesity.x = value;
	_lightData[entity.lightID].update = true;
}

void Light::SetPosition(Entity & entity, XMFLOAT4 & position)
{
	_transform->_transforms[entity.transformID].position = position;
	_lightData[entity.lightID].buffer->position = position;
	_lightData[entity.lightID].update = true;
}

void Light::SetDirection(Entity & entity, XMFLOAT4 & direction)
{
	_lightData[entity.lightID].buffer->direction = direction;
	_transform->_transforms[entity.transformID].direction = direction;
	XMVECTOR vDirection = XMLoadFloat4(&direction);
	XMMATRIX rotMatrix = XMMatrixInverse(NULL, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), vDirection, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&_transform->_transforms[entity.transformID].rotation, rotMatrix);

	XMStoreFloat4(&_transform->_transforms[entity.transformID].direction, XMVector4Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotMatrix));
	XMStoreFloat4(&_transform->_transforms[entity.transformID].up, XMVector4Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotMatrix));
	XMStoreFloat4(&_transform->_transforms[entity.transformID].right, XMVector4Transform(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotMatrix));

	_lightData[entity.lightID].update = true;
}