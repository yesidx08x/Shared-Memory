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
	EntityHandler* _entityHandler;
	Entity* _mainCamera;
	Entity* _skybox;
};