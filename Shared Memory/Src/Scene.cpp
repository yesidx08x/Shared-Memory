#include "Scene.h"

Scene::Scene()
{
	// Always create a scene with a Entityhandler, camera and a skybox
	_entityHandler = new EntityHandler();
	_mainCamera = _entityHandler->CreateEntity("MainCamera");
	_entityHandler->GetTransform()->SetPosition(*_mainCamera, XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f));
	_entityHandler->GetCamera()->BindCamera(*_mainCamera, XM_PI / 3.0, ScreenWidth / ScreenHeight, 0.1f, 1000.0f, true);

	_skybox = _entityHandler->CreateEntity("Skybox");
	_entityHandler->GetShader()->BindShaders(*_skybox, "Cubemap", true, false, false, false, true);
	_entityHandler->GetMesh()->BindMesh(*_skybox, "Standards/InverseSphere.grf");
	_entityHandler->GetMaterial()->BindMaterial(*_skybox, "WellsRadiance.dds", radience);
	_entityHandler->GetTransform()->SetPosScaleRot(*_skybox, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(500.0f, 500.0f, 500.0f));
}

Scene::~Scene()
{
	delete _entityHandler;
}

EntityHandler* Scene::GetEntityHandler(){return _entityHandler;}