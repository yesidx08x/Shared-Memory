#pragma once
#include "DataStructures.h"

struct Entity;

class Transform
{
	friend class EntityHandler;
	friend class Camera;
	friend class Light;
public:

	Transform();
	~Transform();

	void BindTransform(Entity& entity, string identifier, 
		XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f), 
		XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f),
		XMFLOAT3 rotationVector = XMFLOAT3(0.0f, 1.0f, 0.0f), float degrees = 0.0f);
	void BindTransform(Entity& entity, TransformData& transformData);
	void BindChild(Entity& parent, Entity& child);
	void RemoveTransform(Entity& entity);
	void RemoveParent(Entity& entity);

	void SetPosition(Entity& entity, XMFLOAT4 position);
	XMFLOAT4 GetPosition(Entity& entity);

	void MoveForward(Entity& entity, float value);
	void MoveBackward(Entity& entity, float value);
	void MoveLeft(Entity& entity, float value);
	void MoveRight(Entity& entity, float value);
	void MoveUp(Entity& entity, float value);
	void MoveDown(Entity& entity, float value);
	void RotatePitch(Entity& entity, float radians);
	void RotateYaw(Entity& entity, float radians);
	void RotateRoll(Entity& entity, float radians);
	void SetRotation(Entity& entity, XMVECTOR vector, float degrees);
	void SetRotation(Entity& entity, XMFLOAT4X4 matrix);
	void SetScale(Entity& entity, XMFLOAT4 scale);
	void SetPosScaleRot(Entity& entity,
		XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f),
		XMFLOAT3 rotationVector = XMFLOAT3(0.0f, 1.0f, 0.0f), float degrees = 0.0f);

private:
	vector<TransformData> _transforms;

	int CreateTransform();
	XMFLOAT4X4 GetEntityTransform(Entity& entity);
};