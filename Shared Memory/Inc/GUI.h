#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

enum PBRChoiceEnum{frostbite, cookTorrance, ue4, PBRChoiceAmount};

class GUI
{
public:
	GUI();
	~GUI();
	void Create(HWND hwnd);
	void Command(HWND hwnd, WPARAM wParam, LPARAM lParam);
	bool IsSharedOpen();

private:
	static bool _openShared;
	static HWND _shareButton;
};