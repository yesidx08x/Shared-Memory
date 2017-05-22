#pragma once
#include "BufferHandler.h"
#include "ShaderHandler.h"
#include "MaterialHandler.h"
#include "GUI.h"
#include "TimerClass.h"
#include "Scene.h"
#include "Entity/EntityHandler.h"
#include <sstream>

#pragma comment (lib, "winmm.lib")

class Engine
{
private:
	enum RastState{cullback, cullfront, cullnone, wireframe};
	void SetLight(int id);
	void SetMaterials(EntityMaterials data);
	void SetTransformBuffer(XMFLOAT4X4 matrix);
	void SetActiveCamera(CameraData* data);
	void SetActiveCameraInfo(CameraData* data);
	void ShowFPS();
	void CreateRasterizers();
	void SetRasterizer(RastState state);
	void HandleJobs();

	void BeginScene();
	void EndScene();

	EntityHandler* _entityHandler = nullptr;

	// Window
	XMFLOAT2 _winDim = XMFLOAT2(0, 0);
	HWND _window;
	HRESULT _hr;

	// D3D
	IDXGISwapChain* _swapChain = nullptr;
	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _devcon = nullptr;
	ID3D11Texture2D* _backBufferTexture = nullptr;
	ID3D11RenderTargetView* _backbuffer = nullptr;
	ID3D11DepthStencilView* _depthStencilView = nullptr;
	D3D11_VIEWPORT _viewPort;
	ID3D11RasterizerState* _cullBackRasterizer = nullptr;
	vector<ID3D11RasterizerState*> _rasterizers;

#ifdef _DEBUG
	ID3D11Debug* _debugDevice = nullptr;
#endif

	ShaderHandler* _shaderHandler = nullptr;
	BufferHandler* _bufferHandler = nullptr;
	MaterialHandler* _materialHandler = nullptr;
	static GUI* _gui;
	TimerClass* _timer = nullptr;

	// Standards
	ID3D11Buffer* _identityMatrix = nullptr;
	ID3D11BlendState* _transparencyState = nullptr;
	vector<XMFLOAT3> _gridData;
	ID3D11Buffer* _gridBuffer = nullptr;
	UINT _gridStrides = sizeof(XMFLOAT3);
	UINT _gridOffsets = 0;

	vector<Scene*> _scenes;
	string _grfPath;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	Engine();
	Engine(HINSTANCE hInstance);
	~Engine();

	void Update(Scene* scene);
	void Render(Scene* scene);
	Scene* CreateScene();
	HWND GetWindow();
	GUI* GetGuiHandler();
};