#include "Entity/EntityHandler.h"
#include "Entity/Mesh.h"

EntityHandler::EntityHandler()
{
	_mesh = new Mesh();
	_shader = new Shader();
	_transform = new Transform();
	_material = new Material();
	_camera = new Camera(_transform);
	_light = new Light(_transform);
}

EntityHandler::~EntityHandler()
{
	delete _mesh;
	delete _shader;
	delete _transform;
	delete _material;
	delete _camera;
	delete _light;

	for (pair<string, Entity*> entity : _entities)
	{
		delete entity.second;
	}
}

Entity* EntityHandler::CreateEntity(string identifier, bool transform)
{
	Entity* entity = new Entity();
	_entities.insert(pair<string, Entity*>(identifier, entity));
	entity->idNumber = (int)_entities.size() - 1;
	entity->identifier = identifier;

	if (transform)
		entity->transformID = _transform->CreateTransform();
	return entity;
}

Entity * EntityHandler::GetEntity(string identifier)
{
	Entity* entity = nullptr;
	try
	{
		entity = _entities.at(identifier);
	}
	catch (const std::out_of_range& e) 
	{
		PrintError("Could not find entity \"" + identifier + "\"");
	}

	return entity;
}

map<string, Entity*> EntityHandler::GetEntities(){return _entities;}

Mesh * EntityHandler::GetMesh(){return _mesh;}

Shader * EntityHandler::GetShader(){return _shader;}

Transform * EntityHandler::GetTransform(){return _transform;}

Material * EntityHandler::GetMaterial(){return _material;}

Camera * EntityHandler::GetCamera(){return _camera;}

Light * EntityHandler::GetLight(){return _light;}

XMFLOAT4X4 EntityHandler::GetEntityTransform(Entity & entity){return _transform->GetEntityTransform(entity);}

EntityHandler::EntityAmounts* EntityHandler::GetEntityAmounts()
{
	//_entityAmounts.mesh = _mesh->GetMeshData()->size();
	_entityAmounts.transform = _transform->_transforms.size();
	return &_entityAmounts;
}

vector<MeshData> EntityHandler::GetMeshJobs(){return *_mesh->GetMeshJobs();}

void EntityHandler::ClearMeshJobs(){_mesh->ClearJobs();}

vector<MaterialData> EntityHandler::GetMaterialJobs(){return _material->GetMaterialJobs();}

void EntityHandler::ClearMaterialJobs(){_material->ClearJobs();}

vector<ShaderData> EntityHandler::GetShaderJobs(){return _shader->GetShaderJobs();}

void EntityHandler::ClearShaderJobs() { _shader->ClearJobs(); }

CameraData * EntityHandler::GetActiveCamera(){return _camera->GetActiveCameraData();}

LightData EntityHandler::GetLightData(Entity & entity){return _light->_lightData[entity.lightID];}
