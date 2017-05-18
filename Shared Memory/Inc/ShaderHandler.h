#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include <string>
#include "Entity/Shader.h"

using namespace DirectX;
using namespace std;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = nullptr; }

class ShaderHandler
{
private:

	ID3D11Device* _device;
	ID3D11DeviceContext* _devcon;
	HRESULT result;
	wstring _directory;

	void CreateSamplers();

	struct D3D11ShaderData
	{
		ID3D11InputLayout* inputLayout = nullptr;
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11HullShader* hullshader = nullptr;
		ID3D11GeometryShader* geometryShader = nullptr;
		ID3D11DomainShader* domainShader = nullptr;
		ID3D11PixelShader* pixelshader = nullptr;
		ID3D11ComputeShader* computeShader = nullptr;

		string identifier;
	};
	vector<D3D11ShaderData> _shaders;
	ID3DBlob* errorMessage = nullptr;
	ID3DBlob* shaderBuffer = nullptr;
	vector<D3D11_INPUT_ELEMENT_DESC> _inputDesc;

	void CreateInputLayout(const string& fileName, D3D11ShaderData& shaderData);
	void CreateVertexShader(const string& fileName, D3D11ShaderData& shaderData);
	void CreateHullShader(const string& fileName, D3D11ShaderData& shaderData);
	void CreateGeometryShader(const string& fileName, D3D11ShaderData& shaderData);
	void CreateDomainShader(const string& fileName, D3D11ShaderData& shaderData);
	void CreatePixelShader(const string& fileName, D3D11ShaderData& shaderData);
	void CreateComputeShader(const string& fileName, D3D11ShaderData& shaderData);

	vector<ID3D11SamplerState*> _samplers;

public:
	ShaderHandler();
	ShaderHandler(ID3D11Device* device, ID3D11DeviceContext* devcon);
	~ShaderHandler();
	void LoadShaders(ShaderData data);
	void SetShaders(int id);
	void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);
};