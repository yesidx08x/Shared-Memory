#pragma once
#include "DataStructures.h"

struct Entity;

class Mesh
{
	friend class EntityHandler;

public:
	Mesh();
	~Mesh();
	void BindMesh(Entity& entity, string path, bool render = true, GRFVersion version = puntb);
	void BindMesh(Entity& entity, MeshData& data);
	void RemoveMesh(Entity& entity);
	void UpdateMesh(Entity& entity, MeshData& data);
	unsigned int GetVertexCount(Entity& entity);
	UINT32* GetVertexSize(Entity& entity);
	bool GetRender(Entity & entity);
	bool GetTranspareny(Entity& entity);

	vector<MeshData>* GetMeshJobs();
	void ClearJobs();

private:
	vector<MeshData> _data;
	vector<MeshData> _jobData;
	string _path;

	void ReadFile(string name, GRFVersion version);
};