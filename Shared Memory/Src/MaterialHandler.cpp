#include "MaterialHandler.h"

MaterialHandler::MaterialHandler(){}

MaterialHandler::MaterialHandler(ID3D11Device * device, ID3D11DeviceContext * devcon)
{
	_device = device;
	_devcon = devcon;
	_directory.append(L"Textures/");
}

MaterialHandler::~MaterialHandler()
{
	for (size_t i = 0; i < _materials.size(); i++)
	{
		if (_materials[i].resource != nullptr)
		{
			int hej = _materials[i].resource->Release();
		}
	}
}

void MaterialHandler::LoadMaterial(MaterialData& data)
{
	for (unsigned int i = 0; i < _materials.size(); i++)
	{
		if (data.identifier == _materials[i].identifier) // Look for already loaded material
		{
			return;
		}
	}

	_materials.push_back(Material());
	_materials.back().identifier = data.identifier;

	wstring path = _directory;
	if (data.type == albedo)
	{
		_materials.back().type = albedo;
		path.append(L"Albedo/");
	}
	else if (data.type == roughness)
	{
		_materials.back().type = roughness;
		path.append(L"Roughness/");
	}
	else if (data.type == metallic)
	{
		_materials.back().type = metallic;
		path.append(L"Metallic/");
	}
	else if (data.type == normal)
	{
		_materials.back().type = normal;
		path.append(L"Normal/");
	}
	else if (data.type == displacement)
	{
		_materials.back().type = displacement;
		path.append(L"Displacement/");
	}
	else if (data.type == radience)
	{
		_materials.back().type = radience;
		path.append(L"Radience/");
	}
	else if (data.type == irradiance)
	{
		_materials.back().type = irradiance;
		path.append(L"Irradience/");
	}
	else if (data.type == ambientocclusion)
	{
		_materials.back().type = ambientocclusion;
		path.append(L"AmbientOcclusion/");
	}

	HRESULT hr;
	if (data.inMap)
	{
		path.append(data.identifier.begin(), data.identifier.end());
		if (data.type != radience && data.type != irradiance)
			hr = CreateWICTextureFromFile(_device, _devcon, path.c_str(), nullptr, &_materials.back().resource);
		else
			hr = CreateDDSTextureFromFile(_device, _devcon, path.c_str(), nullptr, &_materials.back().resource);
	}
	else
	{
		wstring fullPath;
		fullPath.append(data.identifier.begin(), data.identifier.end());
		if (data.type != radience && data.type != irradiance)
			hr = CreateWICTextureFromFile(_device, _devcon, fullPath.c_str(), nullptr, &_materials.back().resource);
		else
			hr = CreateDDSTextureFromFile(_device, _devcon, fullPath.c_str(), nullptr, &_materials.back().resource);
	}

#ifdef _DEBUG
	if (hr != S_OK)
		PrintError("Could not load texture \"" + data.identifier + "\"");
	else
		PrintSuccess("Loaded texture \"" + data.identifier + "\"");
#endif
}

ID3D11ShaderResourceView** MaterialHandler::GetMaterial(unsigned int id){return &_materials[id].resource;}