#pragma once
#include "Entity/EntityHandler.h"

class Scene
{
public:
	Scene();
	~Scene();
	EntityHandler* GetEntityHandler();

private:
	EntityHandler* _entityHandler = nullptr;
	Entity* _mainCamera = nullptr;
	Entity* _skybox = nullptr;
};