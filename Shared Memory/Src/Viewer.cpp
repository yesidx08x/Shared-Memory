#include "Viewer.h"

Viewer::Viewer(){}

Viewer::Viewer(HINSTANCE hInstance)
{
	_engine = new Engine(hInstance);
	_fbxConverter = new FBXConverter("../FBX/", "../Exported GRF/");
	_dataHandler = new MDataHandler();
	_wndHandle = _engine->GetWindow();
}

Viewer::~Viewer()
{
	delete _fbxConverter;
	delete _dataHandler;
	delete _engine;
}

int Viewer::Run(int nCmdShow)
{
	PopulateScene();
	MSG msg = { 0 };

	// Return when application exit
	if (_wndHandle)
	{
		ShowWindow(_wndHandle, nCmdShow);
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
				_engine->Update(_scene);
				_engine->Render(_scene);
			}
		}
		DestroyWindow(_wndHandle);
	}
	return (int)msg.wParam;
}

void Viewer::PopulateScene()
{
	// Add entities to the scene
	_scene = _engine->CreateScene();
	_entityHandler = _scene->GetEntityHandler();

	// Ligts
	Entity* sun = _entityHandler->CreateEntity("Sun");
	_entityHandler->GetLight()->BindLight(*sun, "Sun", point);
	_entityHandler->GetLight()->SetPosition(*sun, XMFLOAT4(10.0f, 10.0f, -10.0f, 1.0f));
	_entityHandler->GetLight()->SetColor(*sun, XMFLOAT4(0.99f, 0.72f, 0.07f, 1.0f));
	_entityHandler->GetLight()->SetIntensity(*sun, 1.0f);

	// Models
	Entity* rustedIron = _entityHandler->CreateEntity("RustedIron");
	_entityHandler->GetMesh()->BindMesh(*rustedIron, "Standards/Sphere.grf");
	_entityHandler->GetShader()->BindShaders(*rustedIron, "PBR", true, false, false, false, true);
	_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "Iron_A.png", albedo);
	_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "Iron_R.png", roughness);
	_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "Iron_M.png", metallic);
	_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "Iron_N.png", normal);
	_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "WellsRadiance.dds", radience);
	_entityHandler->GetMaterial()->BindMaterial(*rustedIron, "WellsIrradiance.dds", irradiance);
	_entityHandler->GetTransform()->SetPosScaleRot(*rustedIron, XMFLOAT3(5.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));
}

void Viewer::Update()
{
	/*
		Handles shared data between maya application and this program.
		Retrieve data from a shared memory that are being handled in SharedMemory.cpp.
		Data structures are also shared in Datastructures.h.
		When a message are being sent by the maya application that something have been updated then
		this program jump to correct case and retrieve the actually data.
		After this it uses the entityhandler to create an object using the newly retrieved data.
	*/

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
				_entityHandler->GetLight()->SetColor(*_mayaEntites[identifier], light->GetData()->buffer->color);
				_entityHandler->GetLight()->SetIntensity(*_mayaEntites[identifier], light->GetData()->buffer->intesity.x);
			}
			else if (header.subType == DESTROY)
			{
				Entity* entity = _entityHandler->GetEntity(identifier);
				_entityHandler->GetLight()->RemoveLight(*entity);
			}
		}
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