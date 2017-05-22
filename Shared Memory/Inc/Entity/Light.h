#pragma once
#include "DataStructures.h"

struct Entity;
class Transform;

class Light
{
	friend class EntityHandler;
	friend class Transform;
public:
	Light();
	Light(Transform* transform);
	~Light();
	void BindLight(Entity& entity, string name, LightType type = point);
	void BindLight(Entity& entity, LightData data);
	void RemoveLight(Entity& entity);
	void SetColor(Entity& entity, XMFLOAT4& color);
	void SetIntensity(Entity& entity, float value);
	void SetPosition(Entity& entity, XMFLOAT4& position);
	void SetDirection(Entity& entity, XMFLOAT4& direction);

private:
	vector<LightData> _lightData;
	Transform* _transform = nullptr;
};