#pragma once
#include <iostream>
#include <string>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>

using namespace std;
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

//inline void PrintError(string text)
//{
//	HANDLE winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleTextAttribute(winHandle, 12);
//	cout << text << "\n";
//	SetConsoleTextAttribute(winHandle, 15);
//}
//
//inline void PrintSuccess(string text)
//{
//	HANDLE winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleTextAttribute(winHandle, 10);
//	cout << text << "\n";
//	SetConsoleTextAttribute(winHandle, 15);
//}

// Headers
struct MeashHeader
{
	unsigned int vertexCount;
};

// Datas
struct MeshData
{
	XMFLOAT3 vertex;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
};