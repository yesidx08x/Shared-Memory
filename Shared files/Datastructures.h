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
	unsigned int type = 0;
	unsigned int subType = 0;
	unsigned int size = 0;
	unsigned int id = 0;
};

struct MeshHeader
{
	unsigned int vertexCount = 0;
	unsigned int transformID = 0;
	unsigned int materialID = 0;


	unsigned int identifierSize = 0;
	unsigned int transformIDSize = 0;
	unsigned int materialIDSize = 0;
};

struct TransformHeader
{
	unsigned int identifierSize = 0;
};

struct SkinnedMeshHeader
{
	unsigned int vertexCount = 0;
	unsigned int transformID = 0;
	unsigned int materialID = 0;
	unsigned int skinClusterID = 0;
};

struct MaterialHeader
{
	unsigned int albedoSize = 0;
	unsigned int roughnessSize = 0;
	unsigned int metallicSize = 0;
	unsigned int normalSize = 0;
	unsigned int displacementSize = 0;
	unsigned int radienceSize = 0;
	unsigned int irradienceSize = 0;
};

struct SkinClusterHeader
{
	unsigned int jointAmount = 0;
	unsigned int jointIndex = 0;
};

struct LightHeader
{
	unsigned int type = 0;
	unsigned int identifierSize = 0;
	unsigned int transformIDSize = 0;
};

// Datas
struct VertexData
{
	XMFLOAT3 vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT2 uv = XMFLOAT2(0.0f, 0.0f);
	XMFLOAT3 normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 tanget = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 bitanget = XMFLOAT3(0.0f, 0.0f, 0.0f);
};

struct SmallVertexData
{
	XMFLOAT3 vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT2 uv = XMFLOAT2(0.0f, 0.0f);
};

struct MeshData
{
	VertexData* vertexData = nullptr;
	SmallVertexData* smallVertexData = nullptr;
	XMFLOAT4* position = nullptr;

	GRFVersion version = GRFVersion::pu;
	unsigned int nrOfUsers = 0;
	string identifier = "";
	unsigned int vertexCount = 0;
	unsigned int vertexSize = 0;
	bool render = true;
	bool update = true;
	bool transparency = false;
};

struct ColorData
{
	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 transparency = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 hasTexture = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
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
	XMFLOAT4 position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 scale = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4X4 rotation = XMFLOAT4X4(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);
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

	string identifier = "";
	unsigned int nrOfUsers = 0;
	unsigned int parent = -1;
};

struct CameraBuffers
{
	XMFLOAT4X4 view = XMFLOAT4X4(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);
	XMFLOAT4X4 projection = XMFLOAT4X4(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);
};

struct CameraInfo
{
	XMFLOAT4 position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 focusPoint = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
};

struct CameraData
{
	CameraBuffers* buffers = nullptr;
	CameraInfo* info = nullptr;
	XMFLOAT4 offset = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);;
	XMFLOAT4 upVector = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);;
	int transformID = 0;
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