#pragma once
#include "BufferHandler.h"
#include "ShaderHandler.h"
#include "MaterialHandler.h"
#include "GUI.h"
#include <Wincodec.h>
#include "TimerClass.h"
#include <sstream>
#include "Scene.h"
#include "Entity/EntityHandler.h"

#pragma comment (lib, "winmm.lib")

class Engine
{
private:
	enum RastState{cullback, cullfront, cullnone, wireframe};
	void SetLight(int id);
	void SetMaterials(EntityMaterials data);
	void SetTransformBuffer(XMFLOAT4X4 matrix);
	TimerClass* GetTimer();
	void Update();
	void SetActiveCamera(CameraData* data);
	void SetActiveCameraInfo(CameraData* data);
	void SetPBRChoice();
	void ShowFPS();
	void CreateRasterizers();
	void SetRasterizer(RastState state);

	void RenderGrid();
	void BeginScene();
	void EndScene();
	void Render(unsigned int size);

	EntityHandler* _entityHandler;

	// Window
	XMFLOAT2 _winDim;
	HWND _window;
	HRESULT _hr;

	// D3D
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _devcon;
	ID3D11Texture2D* _backBufferTexture;
	ID3D11RenderTargetView* _backbuffer;
	ID3D11DepthStencilView* _depthStencilView;
	D3D11_VIEWPORT _viewPort;
	ID3D11RasterizerState* _cullBackRasterizer;
	vector<ID3D11RasterizerState*> _rasterizers;

	ShaderHandler* _shaderHandler;
	BufferHandler* _bufferHandler;
	MaterialHandler* _materialHandler;
	static GUI* _gui;
	TimerClass* _timer;

	// Standards
	ID3D11Buffer* _identityMatrix;
	ID3D11BlendState* _transparencyState;
	vector<XMFLOAT3> _gridData;
	ID3D11Buffer* _gridBuffer;
	UINT _gridStrides = sizeof(XMFLOAT3);
	UINT _gridOffsets = 0;

	vector<Scene*> _scenes;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	Engine();
	Engine(HINSTANCE hInstance);
	~Engine();

	void Run(Scene* scene);
	void Update(Scene* scene);
	void Render(Scene* scene);
	void HandleJobs();
	Scene* CreateScene();
	HWND GetWindow();
	XMFLOAT2 GetWinDim();
	void CreateGrid(unsigned int size = 10, float spacing = 50.f, XMFLOAT3 origo = XMFLOAT3(0.0f, 0.0f, 0.0f));
	GUI* GetGuiHandler();
};