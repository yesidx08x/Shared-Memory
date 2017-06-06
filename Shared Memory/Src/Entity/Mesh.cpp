#include "Entity/Mesh.h"
#include "Entity/EntityHandler.h"

Mesh::Mesh()
{
	_path = "GRF/";

	MeshData meshData;
	meshData.position = (XMFLOAT4*)_aligned_malloc(sizeof(XMFLOAT4), 16);
	meshData.identifier = "Point";
	meshData.nrOfUsers = INT_MAX;
	meshData.update = false;
	meshData.render = true;
	meshData.transparency = true;
	meshData.vertexCount = 1;
	meshData.vertexSize = sizeof(XMFLOAT4);
	_data.push_back(meshData);
	_jobData.push_back(_data.back());

	MeshData meshData1;
	meshData1.identifier = "Standards/2PlaneCross.grf";
	meshData1.nrOfUsers = INT_MAX;
	meshData1.update = false;
	meshData1.render = true;
	meshData1.transparency = true;
	meshData1.vertexSize = sizeof(SmallVertexData);
	_data.push_back(meshData1);
	ReadFile(meshData1.identifier, pu);
	_jobData.push_back(_data.back());
}

Mesh::~Mesh()
{
	for (size_t i = 0; i < _data.size(); i++)
	{
		_data[i].smallVertexData == nullptr ? 0 : _aligned_free(_data[i].smallVertexData);
		_data[i].vertexData == nullptr ? 0 : _aligned_free(_data[i].vertexData);
		_data[i].position == nullptr ? 0 : _aligned_free(_data[i].position);
	}
}

void Mesh::BindMesh(Entity& entity, string name, bool render, GRFVersion version)
{
	for (unsigned int i = 0; i < _data.size(); i++)
	{
		if (name == _data[i].identifier && _data[i].nrOfUsers > 0) // Look for an already loaded mesh
		{
			entity.meshID = i;
			_data[i].nrOfUsers++;
			return;
		}
	}
	_data.push_back(MeshData());
	entity.meshID = (int)_data.size() - 1;
	_data[entity.meshID].vertexSize = sizeof(VertexData);
	_data[entity.meshID].identifier = name;
	_data[entity.meshID].nrOfUsers = 1;
	_data[entity.meshID].render = render;
	_data[entity.meshID].update = false;
	_data[entity.meshID].transparency = false;
	_data[entity.meshID].version = version;
	ReadFile(name, version);

	_jobData.push_back(_data[entity.meshID]);
}

void Mesh::BindMesh(Entity & entity, MeshData & data)
{
	for (unsigned int i = 0; i < _data.size(); i++)
	{
		if (data.identifier == _data[i].identifier && _data[i].nrOfUsers > 0) // Look for an already loaded mesh
		{
			entity.meshID = i;
			_data[i].nrOfUsers++;
			return;
		}
	}
	_data.push_back(MeshData());
	entity.meshID = (int)_data.size() - 1;

	_data[entity.meshID].vertexCount = data.vertexCount;
	//_data[entity.meshID].vertexData = new VertexData[_data[entity.meshID].vertexCount];
	_data[entity.meshID].vertexData = (VertexData*)_aligned_malloc(sizeof(VertexData) * _data[entity.meshID].vertexCount, 16);
	_data[entity.meshID].vertexSize = sizeof(VertexData);
	_data[entity.meshID].vertexData = data.vertexData;
	_data[entity.meshID].identifier = data.identifier;
	_data[entity.meshID].nrOfUsers = 1;
	_data[entity.meshID].render = true;
	_data[entity.meshID].update = false;
	_data[entity.meshID].transparency = false;
	_data[entity.meshID].version = puntb;
	_jobData.push_back(_data[entity.meshID]);
}

void Mesh::RemoveMesh(Entity & entity)
{
	if (entity.meshID >= 0)
	{
		_data[entity.meshID].nrOfUsers--;
		if (_data[entity.meshID].nrOfUsers <= 0)
		{
			delete _data[entity.meshID].vertexData;
			_data[entity.meshID].nrOfUsers = 0;
			_data[entity.meshID].render = false;
			_data[entity.meshID].update = false;
			_data[entity.meshID].transparency = false;
			_data[entity.meshID].vertexCount = 0;
			_data[entity.meshID].identifier = "";

			_jobData.push_back(_data[entity.meshID]);
		}
		entity.meshID = -1;
	}
}

void Mesh::UpdateMesh(Entity& entity, MeshData & data)
{
	if (entity.meshID >= 0)
	{
		if (_data[entity.meshID].nrOfUsers > 0)
		{
			_data[entity.meshID].update = true;
			_data[entity.meshID].vertexData = data.vertexData;
			_data[entity.meshID].vertexCount = data.vertexCount;
			_jobData.push_back(_data[entity.meshID]);
		}
	}
}

unsigned int Mesh::GetVertexCount(Entity & entity){return _data[entity.meshID].vertexCount;}

UINT32* Mesh::GetVertexSize(Entity & entity){ return &_data[entity.meshID].vertexSize; }

bool Mesh::GetRender(Entity & entity){return _data[entity.meshID].render;}

bool Mesh::GetTranspareny(Entity & entity){return _data[entity.meshID].transparency;}

vector<MeshData>* Mesh::GetMeshJobs(){return &_jobData;}

void Mesh::ClearJobs(){_jobData.clear();}

void Mesh::ReadFile(string name, GRFVersion version)
{
	ifstream infile;
	bool status = false;
	infile.open(_path + name, std::ifstream::binary);
	if (infile.is_open())
	{
		infile.read((char*)&_data.back().vertexCount, sizeof(unsigned int));

		switch (version)
		{
		case puntb:
		{
			_data.back().vertexData = (VertexData*)_aligned_malloc(sizeof(VertexData) * _data.back().vertexCount, 16);
			infile.read((char*)_data.back().vertexData, _data.back().vertexSize * _data.back().vertexCount);
		}
			break;
		case pu:
		{
			_data.back().smallVertexData = (SmallVertexData*)_aligned_malloc(sizeof(SmallVertexData) * _data.back().vertexCount, 16);
			infile.read((char*)_data.back().smallVertexData, _data.back().vertexSize * _data.back().vertexCount);
		}
			break;
		default:
			break;
		}
		status = true;
	}
	infile.close();

	if(status)
		PrintSuccess("Imported \"" + name + "\"");
	else
		PrintError("Could not find path \"" + name + "\"");
}