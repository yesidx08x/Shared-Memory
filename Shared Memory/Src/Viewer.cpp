#include "Viewer.h"

Viewer::Viewer(){}

Viewer::Viewer(HINSTANCE hInstance, int nCmdShow)
{
	_engine = new Engine(hInstance);
	_wndHandle = _engine->GetWindow();
	_nCmdShow = nCmdShow;
	_fbxReader = new FBXReader("C:/Users/stahl/Visual Studio 2015/Team Service/PBR_Shared/Exported files/",
		"C:/Users/stahl/Visual Studio 2015/Team Service/PBR_Shared/Exported files/");
	_dataHandler = new MDataHandler();
}

Viewer::~Viewer()
{
	delete _engine;
}

int Viewer::Run()
{
	MSG msg = { 0 };
	PopulateScene();

	if (_wndHandle)
	{
		ShowWindow(_wndHandle, _nCmdShow);
		while (WM_QUIT != msg.message && !GetAsyncKeyState(VK_ESCAPE))
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Update();
				_engine->Run(_scene);
			}
		}
		DestroyWindow(_wndHandle);
	}
	return (int)msg.wParam;
}

void Viewer::PopulateScene()
{
	//_fbxReader->ConvertFile("Gun", puntb);

	_scene = _engine->CreateScene();
	_entityHandler = _scene->GetEntityHandler();
	//_entityHandler->GetCamera()->UseSharedCamera(true);

	// Ligts
	Entity* sun = _entityHandler->CreateEntity("Sun");
	_entityHandler->GetLight()->BindLight(*sun, "Sun", point);
	_entityHandler->GetLight()->SetPosition(*sun, XMFLOAT4(10.0f, 10.0f, -10.0f, 1.0f));
	_entityHandler->GetLight()->SetColor(*sun, XMFLOAT4(0.99f, 0.72f, 0.07f, 1.0f));
	//_entityHandler->GetLight()->SetColor(*sun, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	_entityHandler->GetLight()->SetIntensity(*sun, 1.0f);

	//Entity* sun2 = _entityHandler->CreateEntity("Sun2");
	//_entityHandler->GetLight()->BindLight(*sun2, "Sun2", point);
	//_entityHandler->GetLight()->SetPosition(*sun2, XMFLOAT4(-10.0f, 10.0f, 10.0f, 1.0f));
	//_entityHandler->GetLight()->SetColor(*sun2, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	//_entityHandler->GetLight()->SetIntensity(*sun2, 1.0f);

	//Entity* directional = _entityHandler->CreateEntity("Directional");
	//_entityHandler->GetLight()->BindLight(*directional, "Directional", LightType::directional);
	//_entityHandler->GetLight()->SetColor(*directional, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	//_entityHandler->GetLight()->SetPosition(*directional, XMFLOAT4(0.0f, 10.0f, -10.0f, 1.0f));
	//_entityHandler->GetLight()->SetDirection(*directional, XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f));
	//_entityHandler->GetLight()->SetIntensity(*directional, 1.0f);

	//// Meshes
	//Entity* ground = _entityHandler->CreateEntity("Ground");
	//_entityHandler->GetMesh()->BindMesh(*ground, "Plane.grf");
	//_entityHandler->GetShader()->BindShaders(*ground, "PBR", true, false, false, false, true);
	//_entityHandler->GetMaterial()->BindMaterial(*ground, "OakFloor_A.png", albedo);
	//_entityHandler->GetMaterial()->BindMaterial(*ground, "OakFloor_R.png", roughness);
	//_entityHandler->GetMaterial()->BindMaterial(*ground, "OakFloor_M.png", metallic);
	//_entityHandler->GetMaterial()->BindMaterial(*ground, "OakFloor_N.png", normal);
	//_entityHandler->GetMaterial()->BindMaterial(*ground, "WellsRadiance.dds", radience);
	//_entityHandler->GetMaterial()->BindMaterial(*ground, "WellsIrradiance.dds", irradiance);
	//_entityHandler->GetTransform()->SetPosScaleRot(*ground, XMFLOAT3(12.5f, -2.5f, 0.0f), XMFLOAT3(35.0f, 35.0f, 35.0f));

	//Entity* gun = _entityHandler->CreateEntity("Gun");
	//_entityHandler->GetMesh()->BindMesh(*gun, "Gun.grf");
	//_entityHandler->GetShader()->BindShaders(*gun, "PBR", true, false, false, false, true);
	//_entityHandler->GetMaterial()->BindMaterial(*gun, "Gun_A.png", albedo);
	//_entityHandler->GetMaterial()->BindMaterial(*gun, "Gun_R.png", roughness);
	//_entityHandler->GetMaterial()->BindMaterial(*gun, "Gun_M.png", metallic);
	//_entityHandler->GetMaterial()->BindMaterial(*gun, "Gun_N.png", normal);
	//_entityHandler->GetMaterial()->BindMaterial(*gun, "WellsRadiance.dds", radience);
	//_entityHandler->GetMaterial()->BindMaterial(*gun, "WellsIrradiance.dds", irradiance);
	//_entityHandler->GetTransform()->SetPosScaleRot(*gun, XMFLOAT3(-15.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT3(0.0f, 1.0f, 0.0f), 270);

	//Entity* rustedIron = _entityHandler->CreateEntity("RustedIron");
	//_entityHandler->GetMesh()->BindMesh(*rustedIron, "Sphere.grf");
	//_entityHandler->GetShader()->BindShaders(*rustedIron, "PBR", true, false, false, false, true);
	//_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "Iron_A.png", albedo);
	//_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "Iron_R.png", roughness);
	//_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "Iron_M.png", metallic);
	//_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "Iron_N.png", normal);
	//_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "WellsRadiance.dds", radience);
	//_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "WellsIrradiance.dds", irradiance);
	//_entityHandler->GetTransform()->SetPosScaleRot(*rustedIron, XMFLOAT3(5.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));

	//Entity* fabric = _entityHandler->CreateEntity("Fabric");
	//_entityHandler->GetMesh()->BindMesh(*fabric, "Sphere.grf");
	//_entityHandler->GetShader()->BindShaders(*fabric, "PBR", true, false, false, false, true);
	//_entityHandler->GetMaterial()->BindMaterial(*fabric, "Fabric_A.png", albedo);
	//_entityHandler->GetMaterial()->BindMaterial(*fabric, "Fabric_R.png", roughness);
	//_entityHandler->GetMaterial()->BindMaterial(*fabric, "Fabric_M.png", metallic);
	//_entityHandler->GetMaterial()->BindMaterial(*fabric, "Fabric_N.png", normal);
	//_entityHandler->GetMaterial()->BindMaterial(*fabric, "Fabric_AO.png", ambientocclusion);
	//_entityHandler->GetMaterial()->BindMaterial(*fabric, "WellsRadiance.dds", radience);
	//_entityHandler->GetMaterial()->BindMaterial(*fabric, "WellsIrradiance.dds", irradiance);
	//_entityHandler->GetTransform()->SetPosScaleRot(*fabric, XMFLOAT3(10.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));

	//Entity* plywood = _entityHandler->CreateEntity("Plywood");
	//_entityHandler->GetMesh()->BindMesh(*plywood, "Sphere.grf");
	//_entityHandler->GetShader()->BindShaders(*plywood, "PBR", true, false, false, false, true);
	//_entityHandler->GetMaterial()->BindMaterial(*plywood, "Plywood_A.png", albedo);
	//_entityHandler->GetMaterial()->BindMaterial(*plywood, "Plywood_R.png", roughness);
	//_entityHandler->GetMaterial()->BindMaterial(*plywood, "Plywood_M.png", metallic);
	//_entityHandler->GetMaterial()->BindMaterial(*plywood, "Plywood_N.png", normal);
	//_entityHandler->GetMaterial()->BindMaterial(*plywood, "Plywood_AO.png", ambientocclusion);
	//_entityHandler->GetMaterial()->BindMaterial(*plywood, "WellsRadiance.dds", radience);
	//_entityHandler->GetMaterial()->BindMaterial(*plywood, "WellsIrradiance.dds", irradiance);
	//_entityHandler->GetTransform()->SetPosScaleRot(*plywood, XMFLOAT3(15.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));

	//Entity* rockCopper = _entityHandler->CreateEntity("RockCopper");
	//_entityHandler->GetMesh()->BindMesh(*rockCopper, "Sphere.grf");
	//_entityHandler->GetShader()->BindShaders(*rockCopper, "PBR", true, false, false, false, true);
	//_entityHandler->GetMaterial()->BindMaterial(*rockCopper, "RockCopper_A.png", albedo);
	//_entityHandler->GetMaterial()->BindMaterial(*rockCopper, "RockCopper_R.png", roughness);
	//_entityHandler->GetMaterial()->BindMaterial(*rockCopper, "RockCopper_M.png", metallic);
	//_entityHandler->GetMaterial()->BindMaterial(*rockCopper, "RockCopper_N.png", normal);
	//_entityHandler->GetMaterial()->BindMaterial(*rockCopper, "RockCopper_AO.png", ambientocclusion);
	//_entityHandler->GetMaterial()->BindMaterial(*rockCopper, "WellsRadiance.dds", radience);
	//_entityHandler->GetMaterial()->BindMaterial(*rockCopper, "WellsIrradiance.dds", irradiance);
	//_entityHandler->GetTransform()->SetPosScaleRot(*rockCopper, XMFLOAT3(20.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));

	//Entity* plastic = _entityHandler->CreateEntity("Plastic");
	//_entityHandler->GetMesh()->BindMesh(*plastic, "Sphere.grf");
	//_entityHandler->GetShader()->BindShaders(*plastic, "PBR", true, false, false, true, true);
	//_entityHandler->GetMaterial()->BindMaterial(*plastic, "Plastic_A.png", albedo);
	//_entityHandler->GetMaterial()->BindMaterial(*plastic, "Plastic_R.png", roughness);
	//_entityHandler->GetMaterial()->BindMaterial(*plastic, "Plastic_M.png", metallic);
	//_entityHandler->GetMaterial()->BindMaterial(*plastic, "Plastic_N.png", normal);
	//_entityHandler->GetMaterial()->BindMaterial(*plastic, "Plastic_AO.png", ambientocclusion);
	//_entityHandler->GetMaterial()->BindMaterial(*plastic, "WellsRadiance.dds", radience);
	//_entityHandler->GetMaterial()->BindMaterial(*plastic, "WellsIrradiance.dds", irradiance);
	//_entityHandler->GetTransform()->SetPosScaleRot(*plastic, XMFLOAT3(25.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));

	//Entity* barrel = _entityHandler->CreateEntity("Barrel");
	//_entityHandler->GetMesh()->BindMesh(*barrel, "Barrel.grf");
	//_entityHandler->GetShader()->BindShaders(*barrel, "PBR", true, false, false, true, true);
	//_entityHandler->GetMaterial()->BindMaterial(*barrel, "Barrel_A.png", albedo);
	//_entityHandler->GetMaterial()->BindMaterial(*barrel, "Barrel_R.png", roughness);
	//_entityHandler->GetMaterial()->BindMaterial(*barrel, "Barrel_M.png", metallic);
	//_entityHandler->GetMaterial()->BindMaterial(*barrel, "Barrel_N.png", normal);
	//_entityHandler->GetMaterial()->BindMaterial(*barrel, "WellsRadiance.dds", radience);
	//_entityHandler->GetMaterial()->BindMaterial(*barrel, "WellsIrradiance.dds", irradiance);
	//_entityHandler->GetTransform()->SetPosScaleRot(*barrel, XMFLOAT3(25.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));

	//Entity* box = _entityHandler->CreateEntity("Box");
	//_entityHandler->GetMesh()->BindMesh(*box, "Box.grf");
	//_entityHandler->GetShader()->BindShaders(*box, "PBR", true, false, false, true, true);
	//_entityHandler->GetMaterial()->BindMaterial(*box, "Box_A.png", albedo);
	//_entityHandler->GetMaterial()->BindMaterial(*box, "Box_R.png", roughness);
	//_entityHandler->GetMaterial()->BindMaterial(*box, "Box_M.png", metallic);
	//_entityHandler->GetMaterial()->BindMaterial(*box, "Box_N.png", normal);
	//_entityHandler->GetMaterial()->BindMaterial(*box, "WellsRadiance.dds", radience);
	//_entityHandler->GetMaterial()->BindMaterial(*box, "WellsIrradiance.dds", irradiance);
	//_entityHandler->GetTransform()->SetPosScaleRot(*box, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));

	_entityAmounts = _entityHandler->GetEntityAmounts();
}

void Viewer::Update()
{
	//Entity* rustedIron = _entityHandler->GetEntity("RustedIron");
	//_entityHandler->GetTransform()->RotateYaw(*rustedIron, 0.00001f);

	if (_dataHandler->SharedOpen() && _engine->GetGuiHandler()->IsSharedOpen())
	{
		DataType type = _dataHandler->Update();
		MessageHeader header = _dataHandler->GetMessageHeader();
		switch (type)
		{
		case CAMERA:
			_entityHandler->GetCamera()->UpdateActiveCamera(*_dataHandler->GetCamera()->GetData());
			break;
		case TRANSFORM:
		{
			MTransform* transform = _dataHandler->GetTransforms()->at(header.id);
			if (header.subType == CREATE)
			{
				Entity* entity = _entityHandler->CreateEntity(transform->GetData()->identifier, false);
				_entityHandler->GetTransform()->BindTransform(*entity, *transform->GetData());
				_mayaEntites.insert(pair<string, Entity*>(transform->GetData()->identifier, entity));
			}
			if (header.subType == WHOLE)
			{
				Entity* entity = _entityHandler->GetEntity(transform->GetData()->identifier);
				_entityHandler->GetTransform()->SetPosition(*entity, transform->GetData()->position);
				_entityHandler->GetTransform()->SetScale(*entity, transform->GetData()->scale);
				_entityHandler->GetTransform()->SetRotation(*entity, transform->GetData()->rotation);
			}
			else if (header.subType == DESTROY)
			{
				Entity* entity = _entityHandler->GetEntity(_dataHandler->GetIdentifier());
				_entityHandler->GetTransform()->RemoveTransform(*entity);
			}
		}
			break;
		case MESH:
		{
			MMesh* mesh = _dataHandler->GetMeshes()->at(header.id);
			if (header.subType == CREATE)
			{
				Entity* entity = _entityHandler->CreateEntity(mesh->GetData()->identifier, false);
				_entityHandler->GetMesh()->BindMesh(*entity, *mesh->GetData());
				_entityHandler->GetShader()->BindShaders(*entity, "PBR", true, false, false, false, true);
				_entityHandler->GetTransform()->BindTransform(*entity, mesh->GetTransformID());
				//_entityHandler->GetLight()->BindLight(*entity, "Sun", point);

				for (size_t i = 0; i < mesh->GetMaterialData().size(); i++)
					_entityHandler->GetMaterial()->BindMaterial(*entity, mesh->GetMaterialData()[i].identifier, mesh->GetMaterialData()[i].type, false);

				_mayaEntites.insert(pair<string, Entity*>(mesh->GetData()->identifier, entity));
			}
			else if (header.subType == WHOLE)
			{
				_entityHandler->GetMesh()->UpdateMesh(*_mayaEntites[mesh->GetData()->identifier], *mesh->GetData());
			}
			else if (header.subType == DESTROY)
			{
				Entity* entity = _entityHandler->GetEntity(_dataHandler->GetIdentifier());
				_entityHandler->GetMesh()->RemoveMesh(*entity);
			}
		}
			break;
		case SKINNEDMESH:
			break;
		case SKINCLUSTER:
			break;
		case LIGHT:
		{
			MLight* light = _dataHandler->GetLights()->at(header.id);
			string identifier = light->GetData()->identifier;
			if (header.subType == CREATE)
			{
				Entity* entity = _entityHandler->CreateEntity(identifier, false);
				_entityHandler->GetLight()->BindLight(*entity, *light->GetData());
				_entityHandler->GetTransform()->BindTransform(*entity, light->GetTransformID());
				_mayaEntites.insert(pair<string, Entity*>(identifier, entity));
			}
			else if (header.subType == WHOLE)
			{
				_entityHandler->GetLight()->SetColor(*_mayaEntites[identifier], light->GetData()->buffer.color);
				_entityHandler->GetLight()->SetIntensity(*_mayaEntites[identifier], light->GetData()->buffer.intesity.x);
			}
			else if (header.subType == DESTROY)
			{
				Entity* entity = _entityHandler->GetEntity(identifier);
				_entityHandler->GetLight()->RemoveLight(*entity);
			}
		}
			break;
		case MATERIAL:
			break;
		case HIERARCHY:
			break;
		case SHUTDOWN:
		{
			for (pair<string, Entity*> entity : _mayaEntites)
			{
				_entityHandler->GetMesh()->RemoveMesh(*entity.second);
				_entityHandler->GetLight()->RemoveLight(*entity.second);
				_entityHandler->GetTransform()->RemoveTransform(*entity.second);
			}			
		}
			break;
		default:
			break;
		}

	}
}