#include "Entity/Shader.h"
#include "Entity/EntityHandler.h"

Shader::Shader()
{
	ShaderData shaderData;
	shaderData.identifier = "Basic";
	shaderData.nrOfUsers = INT_MAX;
	shaderData.vertexShader = true;
	shaderData.hullShader = false;
	shaderData.domainShader = false;
	shaderData.geometryShader = false;
	shaderData.pixelShader = true;
	_shaders.push_back(shaderData);
	_jobs.push_back(shaderData);

	shaderData.identifier = "Billboard";
	shaderData.nrOfUsers = INT_MAX;
	shaderData.vertexShader = true;
	shaderData.hullShader = false;
	shaderData.domainShader = false;
	shaderData.geometryShader = true;
	shaderData.pixelShader = true;
	_shaders.push_back(shaderData);
	_jobs.push_back(shaderData);

	shaderData.identifier = "DirectionalLight";
	shaderData.nrOfUsers = INT_MAX;
	shaderData.vertexShader = true;
	shaderData.hullShader = false;
	shaderData.domainShader = false;
	shaderData.geometryShader = false;
	shaderData.pixelShader = true;
	_shaders.push_back(shaderData);
	_jobs.push_back(shaderData);
}

Shader::~Shader(){}

void Shader::BindShaders(Entity & entity, std::string shaderName, bool vS, bool hS, bool dS, bool gS, bool pS)
{
	for (unsigned int i = 0; i < _shaders.size(); i++)
	{
		if (shaderName == _shaders[i].identifier && _shaders[i].nrOfUsers > 0) // Look for an already loaded shader
		{
			entity.shaderID = i;
			_shaders[i].nrOfUsers++;
			return;
		}
	}
	entity.shaderID = (int)_shaders.size();

	ShaderData shaderData;
	shaderData.identifier = shaderName;
	shaderData.nrOfUsers = 1;

	shaderData.vertexShader = vS;
	shaderData.hullShader = hS;
	shaderData.domainShader = dS;
	shaderData.geometryShader = gS;
	shaderData.pixelShader = pS;

	_shaders.push_back(shaderData);
	_jobs.push_back(shaderData);
}

void Shader::RemoveShaders(Entity & entity)
{
	if (entity.shaderID >= 0)
	{
		_shaders[entity.shaderID].nrOfUsers--;
		if (_shaders[entity.shaderID].nrOfUsers <= 0)
		{
			_shaders[entity.shaderID].vertexShader = false;
			_shaders[entity.shaderID].hullShader = false;
			_shaders[entity.shaderID].domainShader = false;
			_shaders[entity.shaderID].geometryShader = false;
			_shaders[entity.shaderID].pixelShader = false;
			_shaders[entity.shaderID].nrOfUsers = 0;
			_shaders[entity.shaderID].identifier = "";

			_jobs.push_back(_shaders[entity.shaderID]);
		}
		entity.meshID = -1;
	}
}

vector<ShaderData> Shader::GetShaderJobs(){	return _jobs;}

void Shader::ClearJobs() { _jobs.clear(); }
