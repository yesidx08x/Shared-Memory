#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include <string>
#include "../DirectXToolkit/Inc/WICTextureLoader.h"
#include "../DirectXToolkit/Inc/DDSTextureLoader.h"
#include "DataStructures.h"

using namespace DirectX;
using namespace std;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class MaterialHandler
{
public:
	struct Material
	{
		string identifier = "";
		ID3D11ShaderResourceView* resource = nullptr;
		MaterialType type;
	};

	MaterialHandler();
	MaterialHandler(ID3D11Device* device, ID3D11DeviceContext* devcon);
	~MaterialHandler();
	void LoadMaterial(MaterialData& data);
	ID3D11ShaderResourceView** GetMaterial(unsigned int id);

private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _devcon;

	vector<Material> _materials;
	wstring _directory;
};