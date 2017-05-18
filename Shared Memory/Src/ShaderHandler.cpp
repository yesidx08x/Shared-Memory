#include "ShaderHandler.h"

ShaderHandler::ShaderHandler(){}

ShaderHandler::ShaderHandler(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	_device = device;
	_devcon = devcon;
	CreateSamplers();
	_directory.append(L"Shaders/");
}

ShaderHandler::~ShaderHandler(){}

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
		CreateVertexShader(data.identifier, d3dShaderData);
	if (data.geometryShader)
		CreateGeometryShader(data.identifier, d3dShaderData);
	if (data.pixelShader)
		CreatePixelShader(data.identifier, d3dShaderData);

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

void ShaderHandler::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology){_devcon->IASetPrimitiveTopology(topology);}

void ShaderHandler::CreateSamplers()
{
	_samplers.resize(1);

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

	HRESULT hr = _device->CreateSamplerState(&samplerDesc, &_samplers[0]);
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
	else if ("Grid" == fileName)
	{
		_inputDesc.push_back({ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
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
	else if ("Billboard" == fileName)
	{
		_inputDesc.push_back({ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	else if ("DirectionalLight" == fileName)
	{
		_inputDesc.push_back({ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		_inputDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}

	result = _device->CreateInputLayout(_inputDesc.data(), (unsigned int)_inputDesc.size(), shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), &shaderData.inputLayout);
	if (FAILED(result))
	{
		PrintError("ShaderHandler::CreateVertexShader: Error creating input layout.");
	}
}

void ShaderHandler::CreateVertexShader(const string& fileName, D3D11ShaderData& shaderData)
{
	wstring name = _directory;
	name.append(fileName.begin(), fileName.end());
	name.append(L"VS.hlsl");
	result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			PrintError("ShaderHandler::CreateVertexShader: " + std::string(static_cast<const char*>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			std::string s(fileName.begin(), fileName.end());
			PrintError("ShaderHandler::CreateVertexShader: Shader file not found. Filename: " + s);
		}
	}

	result = _device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shaderData.vertexShader);
	if (FAILED(result))
	{
		std::string s(fileName.begin(), fileName.end());
		PrintError("ShaderHandler::CreateVertexShader: Shader file not found. Filename: " + s);
	}

	CreateInputLayout(fileName, shaderData);
}

void ShaderHandler::CreateHullShader(const string& fileName, D3D11ShaderData& shaderData)
{
	wstring name = _directory;
	name.append(fileName.begin(), fileName.end());
	name.append(L"HS.hlsl");
	result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			PrintError("ShaderHandler::CreateHullShader: " + std::string(static_cast<const char*>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			std::string s(fileName.begin(), fileName.end());
			PrintError("ShaderHandler::CreateHullShader: Shader file not found. Filename: " + s);
		}
	}

	result = _device->CreateHullShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shaderData.hullshader);
	if (FAILED(result))
	{
		std::string s(fileName.begin(), fileName.end());
		PrintError("ShaderHandler::CreateHullShader: Shader file not found. Filename: " + s);
	}

	errorMessage->Release();
	shaderBuffer->Release();
}

void ShaderHandler::CreateGeometryShader(const string& fileName, D3D11ShaderData& shaderData)
{
	wstring name = _directory;
	name.append(fileName.begin(), fileName.end());
	name.append(L"GS.hlsl");
	result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			PrintError("ShaderHandler::CreateGeometryShader: " + std::string(static_cast<const char*>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			std::string s(fileName.begin(), fileName.end());
			PrintError("ShaderHandler::CreateGeometryShader: Shader file not found. Filename: " + s);
		}
	}

	result = _device->CreateGeometryShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shaderData.geometryShader);
	if (FAILED(result))
	{
		std::string s(fileName.begin(), fileName.end());
		PrintError("ShaderHandler::CreateGeometryShader: Shader file not found.. Filename: " + s);
	}

	SAFE_RELEASE(errorMessage);
	SAFE_RELEASE(shaderBuffer);
}

void ShaderHandler::CreateDomainShader(const string& fileName, D3D11ShaderData& shaderData)
{
	wstring name = _directory;
	name.append(fileName.begin(), fileName.end());
	name.append(L"DS.hlsl");
	result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			PrintError("ShaderHandler::CreateDomainShader: " + std::string(static_cast<const char*>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			std::string s(fileName.begin(), fileName.end());
			PrintError("ShaderHandler::CreateDomainShader: Shader file not found. Filename: " + s);
		}
	}

	result = _device->CreateDomainShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shaderData.domainShader);
	if (FAILED(result))
	{
		std::string s(fileName.begin(), fileName.end());
		PrintError("ShaderHandler::CreateDomainShader: Shader file not found. Filename: " + s);
	}

	SAFE_RELEASE(errorMessage);
	SAFE_RELEASE(shaderBuffer);
}

void ShaderHandler::CreatePixelShader(const string& fileName, D3D11ShaderData& shaderData)
{
	wstring name = _directory;
	name.append(fileName.begin(), fileName.end());
	name.append(L"PS.hlsl");
	result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, &errorMessage);
	//result = D3DCompileFromFile(fileName, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &shaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			PrintError("ShaderHandler::CreatePixelShader: " + std::string(static_cast<const char*>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			std::string s(fileName.begin(), fileName.end());
			PrintError("ShaderHandler::CreatePixelShader: Shader file not found. Filename: " + s);
		}
	}

	result = _device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shaderData.pixelshader);
	if (FAILED(result))
	{
		std::string s(fileName.begin(), fileName.end());
		PrintError("ShaderHandler::CreatePixelShader: Shader file not found. Filename: " + s);
	}

	SAFE_RELEASE(errorMessage);
	SAFE_RELEASE(shaderBuffer);
}

void ShaderHandler::CreateComputeShader(const string& fileName, D3D11ShaderData& shaderData)
{
	wstring name = _directory;
	name.append(fileName.begin(), fileName.end());
	name.append(L"CS.hlsl");
	result = D3DCompileFromFile(name.c_str(), NULL, NULL, "main", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			PrintError("ShaderHandler::CreateComputeShader: " + std::string(static_cast<const char*>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			std::string s(fileName.begin(), fileName.end());
			PrintError("ShaderHandler::CreateComputeShader: Shader file not found. Filename: " + s);
		}
	}

	result = _device->CreateComputeShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shaderData.computeShader);
	if (FAILED(result))
	{
		std::string s(fileName.begin(), fileName.end());
		PrintError("ShaderHandler::CreateComputeShader: Shader file not found. Filename: " + s);
	}

	SAFE_RELEASE(errorMessage);
	SAFE_RELEASE(shaderBuffer);
}