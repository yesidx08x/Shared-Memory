#pragma once
#include <iostream>
#include <string>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include <map>

using namespace std;
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define ScreenWidth 960.0f
#define ScreenHeight 540.0f
//#define ScreenWidth 1920.0f
//#define ScreenHeight 1080.0f

const unsigned int LighBufferSize = 20;

inline void PrintError(string text)
{
#ifdef _DEBUG
	HANDLE winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(winHandle, 12);
	cout << text << "\n";
	SetConsoleTextAttribute(winHandle, 15);
#endif
}

inline void PrintSuccess(string text)
{
#ifdef _DEBUG
	HANDLE winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(winHandle, 10);
	cout << text << "\n";
	SetConsoleTextAttribute(winHandle, 15);
#endif
}

// Enumerations
enum DataType { CAMERA, TRANSFORM, MESH, SKINNEDMESH, SKINCLUSTER, LIGHT, MATERIAL, HIERARCHY, SHUTDOWN, DATATYPEAMOUNT };
enum PackageType {WHOLE, CREATE, NONE, TEXNAME, TRANSFORMID, MATERIALID, DESTROY, TRANSFORMUPDATE, PACKAGETYPEAMOUNT};
enum MaterialType { albedo, roughness, metallic, normal, displacement, radience, irradiance, ambientocclusion, color };
enum ShaderType { Vertex, Hull, Geometry, Domain, Pixel, Compute };
enum LightType { point, directional, spot };
enum GRFVersion { puntb, pu };

// Headers
struct MessageHeader
{
	unsigned int type;
	unsigned int subType;
	unsigned int size;
	unsigned int id;
};

struct MeshHeader
{
	unsigned int vertexCount;
	unsigned int transformID;
	unsigned int materialID;


	unsigned int identifierSize;
	unsigned int transformIDSize;
	unsigned int materialIDSize;
};

struct TransformHeader
{
	unsigned int identifierSize;
};

struct SkinnedMeshHeader
{
	unsigned int vertexCount;
	unsigned int transformID;
	unsigned int materialID;
	unsigned int skinClusterID;
};

struct MaterialHeader
{
	unsigned int albedoSize;
	unsigned int roughnessSize;
	unsigned int metallicSize;
	unsigned int normalSize;
	unsigned int displacementSize;
	unsigned int radienceSize;
	unsigned int irradienceSize;
};

struct SkinClusterHeader
{
	unsigned int jointAmount;
	unsigned int jointIndex;
};

struct LightHeader
{
	unsigned int type;
	unsigned int identifierSize;
	unsigned int transformIDSize;
};

// Datas
struct VertexData
{
	XMFLOAT3 vertex;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tanget;
	XMFLOAT3 bitanget;
};

struct SmallVertexData
{
	XMFLOAT3 vertex;
	XMFLOAT2 uv;
};

struct MeshData
{
	VertexData* vertexData = nullptr;
	SmallVertexData* smallVertexData = nullptr;
	XMFLOAT4* position = nullptr;

	GRFVersion version;
	unsigned int nrOfUsers = 0;
	string identifier = "";
	unsigned int vertexCount;
	unsigned int vertexSize;
	bool render;
	bool update;
	bool transparency;
};

struct ColorData
{
	XMFLOAT4 color;
	XMFLOAT4 transparency;
	XMFLOAT4 ambient;
	XMFLOAT4 hasTexture;
};

struct MaterialData
{
	string identifier = "";
	MaterialType type;
	ColorData color;
	unsigned int nrOfUsers = 0;
	bool inMap = true;
};

struct MatrixData
{
	XMFLOAT4 position;
	XMFLOAT4 scale;
	XMFLOAT4X4 rotation;
};

struct TransformData
{
	TransformData()
	{
		position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		scale = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		XMStoreFloat4x4(&rotation, XMMatrixIdentity());

		direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
		right = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	}

	XMFLOAT4 position; // (0, 0, 0) as standard
	XMFLOAT4 scale; // (1, 1, 1) as standard
	XMFLOAT4X4 rotation; // identity matrix as standard

	XMFLOAT4 direction; // (0, 0, 1) as standard
	XMFLOAT4 right; // (-1, 0, 0) as standard
	XMFLOAT4 up; // (0, 1, 0) as standard

	MatrixData transform;

	string identifier;
	unsigned int nrOfUsers = 0;
	unsigned int parent = -1;
};

struct PBRChoice
{
	XMINT4 brdf;
};

struct CameraBuffers
{
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

struct CameraInfo
{
	XMFLOAT4 position;
	XMFLOAT4 focusPoint;
};

struct CameraData
{
	CameraBuffers* buffers = nullptr;
	CameraInfo* info;
	XMFLOAT4 offset;
	XMFLOAT4 upVector;
	int transformID;
};

struct LightBuffer
{
	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 direction = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	XMFLOAT4 position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 intesity = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	int active = 0;
};

struct LightData
{
	LightBuffer* buffer = nullptr;
	LightType type = point;
	string identifier = "";
	unsigned int nrOfUsers = 0;
	bool update = false;
};

struct MeshSkinnedData
{
	XMFLOAT3 vertex;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT4 boneIndices;
	XMFLOAT4 boneWeights;
};

struct SkinClusterData
{
	XMFLOAT4X4 matrix;
};