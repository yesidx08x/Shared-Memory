#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include "Entity/Shader.h"

using namespace DirectX;
using namespace std;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class ShaderHandler
{
public:
	ShaderHandler();
	ShaderHandler(ID3D11Device* device, ID3D11DeviceContext* devcon);
	~ShaderHandler();
	void LoadShaders(ShaderData data);
	void SetShaders(int id);
	void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);

private:
	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _devcon = nullptr;
	HRESULT _result = S_OK;
	wstring _directory = L"";

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
	vector<ID3D11SamplerState*> _samplers;
	ID3DBlob* _errorMessage = nullptr;
	ID3DBlob* _shaderBuffer = nullptr;
	vector<D3D11_INPUT_ELEMENT_DESC> _inputDesc;

	void CreateInputLayout(const string& fileName, D3D11ShaderData& shaderData);
	void CreateShader(const string& fileName, D3D11ShaderData& shaderData, ShaderType shaderType);
	void HandleShaderError(HRESULT result, string name);
};