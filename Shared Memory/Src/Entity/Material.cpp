#include "Entity/Material.h"
#include "Entity/EntityHandler.h"

Material::Material()
{
	MaterialData materialData;
	materialData.identifier = "Temp_A.png";
	materialData.type = albedo;
	materialData.nrOfUsers = INT_MAX;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);

	materialData.identifier = "Temp_R.png";
	materialData.type = roughness;
	materialData.nrOfUsers = INT_MAX;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);

	materialData.identifier = "Temp_M.png";
	materialData.type = metallic;
	materialData.nrOfUsers = INT_MAX;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);

	materialData.identifier = "Temp_N.png";
	materialData.type = normal;
	materialData.nrOfUsers = INT_MAX;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);

	materialData.identifier = "Temp_D.png";
	materialData.type = displacement;
	materialData.nrOfUsers = INT_MAX;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);

	materialData.identifier = "Temp_Ra.dds";
	materialData.type = radience;
	materialData.nrOfUsers = INT_MAX;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);

	materialData.identifier = "Temp_I.dds";
	materialData.type = irradiance;
	materialData.nrOfUsers = INT_MAX;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);

	materialData.identifier = "Temp_AO.png";
	materialData.type = ambientocclusion;
	materialData.nrOfUsers = INT_MAX;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);

	// Specials
	materialData.identifier = "PointLightBillboard_A.png";
	materialData.type = albedo;
	materialData.nrOfUsers = INT_MAX;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);

	materialData.identifier = "DirectionalLightBillboard_A.png";
	materialData.type = albedo;
	materialData.nrOfUsers = INT_MAX;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);
}

Material::~Material(){}

void Material::BindMaterial(Entity & entity, std::string fileName, MaterialType type, bool inMap)
{
	for (unsigned int i = 0; i < _materials.size(); i++)
	{
		if (fileName == _materials[i].identifier && _materials[i].nrOfUsers > 0) // Look for an already loaded material
		{

			if (type == albedo)
			{
				entity.materialsIDs.albedo = i;
			}
			else if (type == roughness)
			{
				entity.materialsIDs.roughness = i;
			}
			else if (type == metallic)
			{
				entity.materialsIDs.metallic = i;
			}
			else if (type == normal)
			{
				entity.materialsIDs.normal = i;
			}
			else if (type == displacement)
			{
				entity.materialsIDs.displacement = i;
			}
			else if (type == radience)
			{
				entity.materialsIDs.radience = i;
			}
			else if (type == irradiance)
			{
				entity.materialsIDs.irradience = i;
			}
			else if (type == ambientocclusion)
			{
				entity.materialsIDs.ambientocclusion = i;
			}

			_materials[i].nrOfUsers++;
			return;
		}
	}

	MaterialData materialData;
	materialData.identifier = fileName;
	unsigned int vectorPos = (unsigned int)_materials.size();

	if (type == albedo)
	{
		entity.materialsIDs.albedo = vectorPos;
		materialData.type = albedo;
	}
	else if (type == roughness)
	{
		entity.materialsIDs.roughness = vectorPos;
		materialData.type = roughness;
	}
	else if (type == metallic)
	{
		entity.materialsIDs.metallic = vectorPos;
		materialData.type = metallic;
	}
	else if (type == normal)
	{
		entity.materialsIDs.normal = vectorPos;
		materialData.type = normal;
	}
	else if (type == displacement)
	{
		entity.materialsIDs.displacement = vectorPos;
		materialData.type = displacement;
	}
	else if (type == radience)
	{
		entity.materialsIDs.radience = vectorPos;
		materialData.type = radience;
	}
	else if (type == irradiance)
	{
		entity.materialsIDs.irradience = vectorPos;
		materialData.type = irradiance;
	}
	else if (type == ambientocclusion)
	{
		entity.materialsIDs.ambientocclusion = vectorPos;
		materialData.type = ambientocclusion;
	}

	materialData.inMap = inMap;
	materialData.nrOfUsers = 1;
	_materials.push_back(materialData);
	_jobMaterials.push_back(materialData);
}

vector<MaterialData> Material::GetMaterialData()
{
	return _materials;
}

vector<MaterialData> Material::GetMaterialJobs(){return _jobMaterials;}

void Material::ClearJobs(){_jobMaterials.clear();}