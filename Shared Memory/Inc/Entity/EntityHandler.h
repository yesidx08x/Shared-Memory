#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"

struct EntityMaterials
{
	unsigned int albedo = 0;
	unsigned int roughness = 1;
	unsigned int metallic = 2;
	unsigned int normal = 3;
	unsigned int displacement = 4;
	unsigned int radience = 5;
	unsigned int irradience = 6;
	unsigned int ambientocclusion = 7;

	bool operator==(EntityMaterials const& other)
	{
		if (this->albedo == other.albedo &&
			this->roughness == other.roughness &&
			this->metallic == other.metallic &&
			this->normal == other.normal &&
			this->displacement == other.displacement &&
			this->radience == other.radience &&
			this->irradience == other.irradience &&
			this->ambientocclusion == other.ambientocclusion)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetMaterials(unsigned int albedo, unsigned int roughness, unsigned int metallic, unsigned int normal, unsigned int displacement, unsigned int radience, unsigned int irradience, unsigned int ambientocclusion)
	{
		this->albedo = albedo;
		this->roughness = roughness;
		this->metallic = metallic;
		this->normal = normal;
		this->displacement = displacement;
		this->radience = radience;
		this->irradience = irradience;
		this->ambientocclusion = ambientocclusion;
	}
};

struct Entity
{
	friend class EntityHandler;
	friend class Engine;
	friend class Mesh;
	friend class Shader;
	friend class Transform;
	friend class Material;
	friend class Camera;
	friend class Light;
private:
	string identifier; // to be able to find a specific entity for for example copying, or binding as child/parent
	int idNumber; // Number given by Entity handler
	Entity* parent = nullptr; // To be able to tell parent that child is removed
	vector<Entity*> children; // Update children with position and direction when parent changes

	int meshID = -1;
	EntityMaterials materialsIDs;
	int shaderID = 0;
	int transformID = -1;
	int cameraID = -1;
	int lightID = -1;

public:

	bool operator==(Entity const& other)
	{
		if (this->identifier == other.identifier && 
			this->idNumber == other.idNumber &&
			this->parent == other.parent &&
			this->children == other.children &&
			this->meshID == other.meshID && 
			this->materialsIDs == other.materialsIDs &&
			this->shaderID == other.shaderID && 
			this->transformID == other.transformID &&
			this->cameraID == other.cameraID && 
			this->lightID == other.lightID)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

};

class EntityHandler
{
	friend class Engine;
public:
	struct EntityAmounts
	{
		unsigned int transform = -1;
		unsigned int mesh = -1;
	};
	EntityHandler();
	~EntityHandler();

	Entity* CreateEntity(string identifier, bool transform = true);
	Entity* GetEntity(string identifier);
	map<string, Entity*> GetEntities();
	Mesh* GetMesh();
	Shader* GetShader();
	Transform* GetTransform();
	Material* GetMaterial();
	Camera* GetCamera();
	Light* GetLight();
	XMFLOAT4X4 GetEntityTransform(Entity& entity);
	EntityAmounts* GetEntityAmounts();

	vector<MeshData> GetMeshJobs();
	void ClearMeshJobs();
	vector<MaterialData> GetMaterialJobs();
	void ClearMaterialJobs();
	vector<ShaderData> GetShaderJobs();
	void ClearShaderJobs();

private:
	EntityAmounts _entityAmounts;
	map<string, Entity*> _entities;
	Mesh* _mesh = nullptr;
	Shader* _shader = nullptr;
	Transform* _transform = nullptr;
	Material* _material = nullptr;
	Camera* _camera = nullptr;
	Light* _light = nullptr;

	CameraData* GetActiveCamera();
	LightData GetLightData(Entity& entity);
};