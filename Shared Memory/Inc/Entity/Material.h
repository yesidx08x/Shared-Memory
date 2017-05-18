#pragma once
#include "DataStructures.h"

struct Entity;

class Material
{
	friend class EntityHandler;
public:
	Material();
	~Material();
	void BindMaterial(Entity& entity, std::string fileName, MaterialType type, bool inMap = true);
	vector<MaterialData> GetMaterialData();
	vector<MaterialData> GetMaterialJobs();
	void ClearJobs();

private:
	vector<MaterialData> _materials;
	vector<MaterialData> _jobMaterials;
};