#pragma once
#include "DataStructures.h"

struct Entity;

struct ShaderData
{
	bool vertexShader = false;
	bool hullShader = false;
	bool domainShader = false;
	bool geometryShader = false;
	bool pixelShader = false;

	unsigned int nrOfUsers = 0;
	string identifier;
};

class Shader
{
	friend class EntityHandler;
public:

	Shader();
	~Shader();
	void BindShaders(Entity& entity, std::string shaderName, bool vS, bool hS, bool dS, bool gS, bool pS);
	void RemoveShaders(Entity& entity);
	vector<ShaderData> GetShaderJobs();
	void ClearJobs();

private:
	vector<ShaderData> _shaders;
	vector<ShaderData> _jobs;
};