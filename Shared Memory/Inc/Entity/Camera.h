#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "DataStructures.h"

using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

struct Entity;
class Transform;

class Camera
{
	friend class EntityHandler;
	friend class Transform;
private:
	struct ActiveCamera
	{
		int cameraID;
		string entityID;
	};
	vector<CameraData> _cameras;
	Transform* _transform;
	TransformData* _activeData;
	ActiveCamera _activeCamera;
	bool _shared;

	float _moveSpeed;
	float _rotationSpeed;

	void GetKeayInput(Entity& entity, float dt);

public:
	Camera();
	Camera(Transform* transform);
	~Camera();
	void BindCamera(Entity& entity, float fov, float aspectRatio, float nearPlane, float farPlane, bool setActive, float moveSpeed = 15.0f, float rotationSpeed = 1.5f);

	CameraData* GetActiveCameraData();
	string GetActiveEntityID();
	void UpdateActiveCamera(Entity& entity, float dt);
	void UpdateActiveCamera(CameraData& data);
	void UseSharedCamera(bool shared);
};