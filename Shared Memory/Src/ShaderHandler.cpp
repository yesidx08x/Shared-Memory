#include "ShaderHandler.h"

ShaderHandler::ShaderHandler() {}

ShaderHandler::ShaderHandler(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	_device = device;
	_devcon = devcon;
	CreateSamplers();
	_directory.append(L"Shaders/");
}

ShaderHandler::~ShaderHandler()
{
	_errorMessage != nullptr ? _errorMessage->Release() : 0;
	_shaderBuffer != nullptr ? _shaderBuffer->Release() : 0;
	for (size_t i = 0; i < _shaders.size(); i++)
	{
		_shaders[i].computeShader != nullptr ? _shaders[i].computeShader->Release() : 0;
		_shaders[i].domainShader != nullptr ? _shaders[i].domainShader->Release() : 0;
		_shaders[i].geometryShader != nullptr ? _shaders[i].geometryShader->Release() : 0;
		_shaders[i].hullshader != nullptr ? _shaders[i].hullshader->Release() : 0;
		_shaders[i].inputLayout != nullptr ? _shaders[i].inputLayout->Release() : 0;
		_shaders[i].pixelshader != nullptr ? _shaders[i].pixelshader->Release() : 0;
		_shaders[i].vertexShader != nullptr ? _shaders[i].vertexShader->Release() : 0;
	}
}

void ShaderHandler::LoadShaders(ShaderData data)
{
	for (unsigned int i = 0; i < _shaders.size(); i++)
	{
		if (data.identifier == _shaders[i].identifier) // Look for already loaded shaders
		{
			return;
		}
	}

	D3D11ShaderData d3dShaderData;
	d3dShaderData.identifier = data.identifier;

	if (data.vertexShader)
		CreateShader(data.identifier, d3dShaderData, Vertex);
	if (data.geometryShader)
		CreateShader(data.identifier, d3dShaderData, Geometry);
	if (data.pixelShader)
		CreateShader(data.identifier, d3dShaderData, Pixel);

	_shaders.push_back(d3dShaderData);
}

void ShaderHandler::SetShaders(int id)
{
	_devcon->PSSetSamplers(0, 1, &_samplers[0]);
	_devcon->IASetInputLayout(_shaders[id].inputLayout);
	_devcon->VSSetShader(_shaders[id].vertexShader, nullptr, 0);
	_devcon->HSSetShader(_shaders[id].hullshader, nullptr, 0);
	_devcon->DSSetShader(_shaders[id].domainShader, nullptr, 0);
	_devcon->GSSetShader(_shaders[id].geometryShader, nullptr, 0);
	_devcon->PSSetShader(_shaders[id].pixelshader, nullptr, 0);
}

void ShaderHandler::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology) { _devcon->IASetPrimitiveTopology(topology); }

void ShaderHandler::CreateSamplers()
{
	_samplers.resize(1);

	// Basic sampler
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = -D3D11_FLOAT32_MAX;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	_result = _device->CreateSamplerState(&samplerDesc, &_samplers[0]);

#ifdef _DEBUG
	if (_result != S_OK)
		PrintError("Could not create sampler");
	else
		PrintSuccess("Created sampler");
#endif
}

void ShaderHandler::CreateInputLayout(const string & fileName, D3D11ShaderData & shaderData)
{
	_inputDesc.clear();
	if ("Basic" == fileName)
	{
		_inputDesc.push_back({ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	else if ("Billboard" == fileName)
	{
		_inputDesc.push_back({ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	else if ("DirectionalLight" == fileName)
	{
		_inputDesc.push_back({ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	else if ("Cubemap" == fileName)
	{
		_inputDesc.push_back({ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	else if ("Lambert" == fileName)
	{
		_inputDesc.push_back({ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	else if ("Phong" == fileName)
	{
		_inputDesc.push_back({ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	else if ("PBR" == fileName)
	{
		_inputDesc.push_back({ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}

	_result = _device->CreateInputLayout(_inputDesc.data(), (unsigned int)_inputDesc.size(), _shaderBuffer->GetBufferPointer(), _shaderBuffer->GetBufferSize(), &shaderData.inputLayout);
#ifdef _DEBUG
	if (FAILED(_result))
	{
		PrintError("ShaderHandler::CreateInputLayout: Error creating input layout.");
	}
#endif
}

void ShaderHandler::CreateShader(const string & fileName, D3D11ShaderData & shaderData, ShaderType shaderType)
{
	wstring name = _directory;
	name.append(fileName.begin(), fileName.end());

	switch (shaderType)
	{
	case Vertex:
		name.append(L"VS.hlsl");
		_result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &_shaderBuffer, &_errorMessage);
		HandleShaderError(_result, fileName);
		_result = _device->CreateVertexShader(_shaderBuffer->GetBufferPointer(), _shaderBuffer->GetBufferSize(), NULL, &shaderData.vertexShader);
		HandleShaderError(_result, fileName);
		CreateInputLayout(fileName, shaderData);
		break;
	case Hull:
		name.append(L"HS.hlsl");
		_result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &_shaderBuffer, &_errorMessage);
		HandleShaderError(_result, fileName);
		_result = _device->CreateHullShader(_shaderBuffer->GetBufferPointer(), _shaderBuffer->GetBufferSize(), NULL, &shaderData.hullshader);
		HandleShaderError(_result, fileName);
		break;
	case Geometry:
		name.append(L"GS.hlsl");
		_result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &_shaderBuffer, &_errorMessage);
		HandleShaderError(_result, fileName);
		_result = _device->CreateGeometryShader(_shaderBuffer->GetBufferPointer(), _shaderBuffer->GetBufferSize(), NULL, &shaderData.geometryShader);
		HandleShaderError(_result, fileName);
		break;
	case Domain:
		name.append(L"DS.hlsl");
		_result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &_shaderBuffer, &_errorMessage);
		HandleShaderError(_result, fileName);
		_result = _device->CreateDomainShader(_shaderBuffer->GetBufferPointer(), _shaderBuffer->GetBufferSize(), NULL, &shaderData.domainShader);
		HandleShaderError(_result, fileName);
		break;
	case Pixel:
		name.append(L"PS.hlsl");
		_result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &_shaderBuffer, &_errorMessage);
		HandleShaderError(_result, fileName);
		_result = _device->CreatePixelShader(_shaderBuffer->GetBufferPointer(), _shaderBuffer->GetBufferSize(), NULL, &shaderData.pixelshader);
		HandleShaderError(_result, fileName);
		break;
	case Compute:
		name.append(L"CS.hlsl");
		_result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &_shaderBuffer, &_errorMessage);
		HandleShaderError(_result, fileName);
		_result = _device->CreateComputeShader(_shaderBuffer->GetBufferPointer(), _shaderBuffer->GetBufferSize(), NULL, &shaderData.computeShader);
		HandleShaderError(_result, fileName);
		break;
	default:
		break;
	}
}

void ShaderHandler::HandleShaderError(HRESULT result, string name)
{
#ifdef _DEBUG
	if (FAILED(_result))
	{
		if (_errorMessage)
			PrintError("ShaderHandler::CreateShader: " + std::string(static_cast<const char*>(_errorMessage->GetBufferPointer()), _errorMessage->GetBufferSize()));
		else
			PrintError("ShaderHandler::CreateShader: Shader file not found. Filename: " + name);
	}
#endif
}