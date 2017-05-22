#pragma once
#include "Entity/EntityHandler.h"

class Scene
{
public:
	Scene();
	~Scene();
	void Update(float dt);
	EntityHandler* GetEntityHandler();

private:
	EntityHandler* _entityHandler = nullptr;
	Entity* _mainCamera = nullptr;
	Entity* _skybox = nullptr;
};