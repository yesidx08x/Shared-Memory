#include "MaterialHandler.h"

MaterialHandler::MaterialHandler(){}

MaterialHandler::MaterialHandler(ID3D11Device * device, ID3D11DeviceContext * devcon)
{
	_device = device;
	_devcon = devcon;
	_directory.append(L"Textures/");
}

MaterialHandler::~MaterialHandler(){}

void MaterialHandler::LoadMaterial(MaterialData& data)
{
	for (unsigned int i = 0; i < _materials.size(); i++)
	{
		if (data.identifier == _materials[i].identifier) // Look for already loaded material
		{
			return;
		}
	}

	Material material;
	material.identifier = data.identifier;
	size_t vectorSpot = _materials.size();

	wstring path = _directory;
	if (data.type == albedo)
	{
		material.type = albedo;
		path.append(L"Albedo/");
	}
	else if (data.type == roughness)
	{
		material.type = roughness;
		path.append(L"Roughness/");
	}
	else if (data.type == metallic)
	{
		material.type = metallic;
		path.append(L"Metallic/");
	}
	else if (data.type == normal)
	{
		material.type = normal;
		path.append(L"Normal/");
	}
	else if (data.type == displacement)
	{
		material.type = displacement;
		path.append(L"Displacement/");
	}
	else if (data.type == radience)
	{
		material.type = radience;
		path.append(L"Radience/");
	}
	else if (data.type == irradiance)
	{
		material.type = irradiance;
		path.append(L"Irradience/");
	}
	else if (data.type == ambientocclusion)
	{
		material.type = ambientocclusion;
		path.append(L"AmbientOcclusion/");
	}

	HRESULT hr;
	if (data.inMap)
	{
		path.append(data.identifier.begin(), data.identifier.end());
		if (data.type != radience && data.type != irradiance)
			hr = CreateWICTextureFromFile(_device, _devcon, path.c_str(), nullptr, &material.resource);
		else
			hr = CreateDDSTextureFromFile(_device, _devcon, path.c_str(), nullptr, &material.resource);
	}
	else
	{
		wstring fullPath;
		fullPath.append(data.identifier.begin(), data.identifier.end());
		if (data.type != radience && data.type != irradiance)
			hr = CreateWICTextureFromFile(_device, _devcon, fullPath.c_str(), nullptr, &material.resource);
		else
			hr = CreateDDSTextureFromFile(_device, _devcon, fullPath.c_str(), nullptr, &material.resource);
	}

	if (hr != S_OK)
		PrintError("Could not load texture \"" + data.identifier + "\"");
	else
		PrintSuccess("Loaded texture \"" + data.identifier + "\"");

	_materials.push_back(material);
}

ID3D11ShaderResourceView** MaterialHandler::GetMaterial(unsigned int id){return &_materials[id].resource;}