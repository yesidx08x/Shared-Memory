#include "Engine.h"

GUI* Engine::_gui;

Engine::Engine(){}

Engine::Engine(HINSTANCE hInstance)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	_winDim = XMFLOAT2(ScreenWidth, ScreenHeight);

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = "Shared_Memory_Viewer";
	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, (LONG)_winDim.x, (LONG)_winDim.y };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	_window = CreateWindow(
		"Shared_Memory_Viewer",
		"Shared Memory Viewer",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	CoInitialize(NULL);

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = _window;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;

	//D3D11_CREATE_DEVICE_DEBUG,
	_hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&_swapChain,
		&_device,
		NULL,
		&_devcon);

	if (SUCCEEDED(_hr))
	{
		// get the address of the back buffer
		_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&_backBufferTexture);

		// use the back buffer address to create the render target
		_device->CreateRenderTargetView(_backBufferTexture, NULL, &_backbuffer);

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.Width = (unsigned int)ScreenWidth;
		depthStencilDesc.Height = (unsigned int)ScreenHeight;
		depthStencilDesc.MipLevels = 0;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.MiscFlags = 0;
		depthStencilDesc.CPUAccessFlags = 0;

		ID3D11Texture2D* pStencilTexture;
		_device->CreateTexture2D(&depthStencilDesc, NULL, &pStencilTexture);
		_device->CreateDepthStencilView(pStencilTexture, NULL, &_depthStencilView);
		pStencilTexture->Release();

		// set the render target as the back buffer
		_devcon->OMSetRenderTargets(1, &_backbuffer, _depthStencilView);
	}

	_viewPort.Width = (float)_winDim.x;
	_viewPort.Height = (float)_winDim.y;
	_viewPort.MinDepth = 0.0f;
	_viewPort.MaxDepth = 1.0f;
	_viewPort.TopLeftX = 0;
	_viewPort.TopLeftY = 0;
	_devcon->RSSetViewports(1, &_viewPort);

	D3D11_BLEND_DESC transparentDesc;
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_device->CreateBlendState(&transparentDesc, &_transparencyState);
	float blendfactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	_devcon->OMSetBlendState(_transparencyState, blendfactor, 0xffffffff);

	CreateRasterizers();

	// Identity matrix
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = &matrix;
	_device->CreateBuffer(&bufferDesc, &subData, &_identityMatrix);

	_shaderHandler = new ShaderHandler(_device, _devcon);
	_bufferHandler = new BufferHandler(_device, _devcon);
	_materialHandler = new MaterialHandler(_device, _devcon);
	


	_timer = new TimerClass(60);

	HRESULT hr;
	ID3D11ShaderResourceView* resource;
	hr = CreateDDSTextureFromFile(_device, L"Textures/Radience/WellsRadiance.dds" , nullptr, &resource, 0);
	ID3D11Texture2D* texture;
	ID3D11Resource *res;
	resource->GetResource(&res);
	hr = res->QueryInterface<ID3D11Texture2D>(&texture);

	unsigned int order = 3;
	vector<float> red;
	vector<float> green;
	vector<float> blue;
	red.resize(order*order);
	green.resize(order*order);
	blue.resize(order*order);

	//ScratchImage image;
	////hr = LoadFromDDSFile(L"Textures/Radience/Lycksele.dds", DDS_FLAGS_NONE, nullptr, image);
	//string cat = "SavedImages/Test.dds";
	//CaptureTexture(_device, _devcon, texture, image);
	////const Image* img = image.GetImage(0, 0, 0);
	//int hej = image.GetImageCount();
	//TexMetadata metaData = image.GetMetadata();
	////SaveToDDSFile(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DDS_FLAGS_NONE, wstring(cat.begin(), cat.end()).c_str());
}

Engine::~Engine()
{
	delete _shaderHandler;
	delete _bufferHandler;
	delete _materialHandler;
}

void Engine::Run(Scene * scene)
{
	if (_timer->FrameRun())
	{
		Update(scene);
		Render(scene);
		ShowFPS();
	}
	_timer->Tick();
}

void Engine::Update(Scene * scene)
{
	_entityHandler = scene->GetEntityHandler();
	HandleJobs();
	scene->Update(_timer->frameDeltaTime());

	_bufferHandler->UpdateBuffer(*_bufferHandler->GetPBRChoice(), (void*)_gui->GetPBRChoice(), sizeof(XMINT4));
	Camera* camera = _entityHandler->GetCamera();
	camera->UpdateActiveCamera(*_entityHandler->GetEntity(_entityHandler->GetCamera()->GetActiveEntityID()), _timer->frameDeltaTime());
	_bufferHandler->UpdateBuffer(*_bufferHandler->GetCamera(), &camera->GetActiveCameraData()->buffers, sizeof(CameraBuffers));
	_bufferHandler->UpdateBuffer(*_bufferHandler->GetCameraInfo(), &camera->GetActiveCameraData()->info, sizeof(CameraInfo));
}

void Engine::Render(Scene * scene)
{
	BeginScene();

	//vector<Entity*> entities = _entityHandler->_entities; // Wrong here
	map<string, Entity*> entities = _entityHandler->GetEntities();

	Camera* camera = _entityHandler->GetCamera();
	SetActiveCamera(camera->GetActiveCameraData());
	SetActiveCameraInfo(camera->GetActiveCameraData());
	SetPBRChoice();
	SetLight(0);
	UINT32 offset = 0;

	// Normal meshes
	SetRasterizer(cullback);
	_shaderHandler->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (pair<string, Entity*> entity : entities) // change this later so things get sorted and unnecessary sets aren't made
	{
		if (entity.second != nullptr)
		{
			if (entity.second->meshID != -1 && _entityHandler->GetMesh()->GetRender(*entity.second) && !_entityHandler->GetMesh()->GetTranspareny(*entity.second))
			{
				_shaderHandler->SetShaders(entity.second->shaderID);
				_devcon->IASetVertexBuffers(0, 1, _bufferHandler->GetVertexBuffer(entity.second->meshID), _entityHandler->GetMesh()->GetVertexSize(*entity.second), &offset);
				SetTransformBuffer(_entityHandler->GetEntityTransform(*entity.second));
				SetMaterials(entity.second->materialsIDs);
				Render(_entityHandler->GetMesh()->GetVertexCount(*entity.second));
			}
		}
	}

	// Transparent objects
	SetRasterizer(cullnone);
	for (pair<string, Entity*> entity : entities) // change this later so things get sorted and unnecessary sets aren't made
	{
		if (entity.second != nullptr)
		{
			if (entity.second->meshID != -1 && _entityHandler->GetMesh()->GetRender(*entity.second) && _entityHandler->GetMesh()->GetTranspareny(*entity.second))
			{
				if(_entityHandler->GetMesh()->GetVertexCount(*entity.second) == 1)
					_shaderHandler->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
				else
					_shaderHandler->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				_shaderHandler->SetShaders(entity.second->shaderID);
				_devcon->IASetVertexBuffers(0, 1, _bufferHandler->GetVertexBuffer(entity.second->meshID), _entityHandler->GetMesh()->GetVertexSize(*entity.second), &offset);
				SetTransformBuffer(_entityHandler->GetEntityTransform(*entity.second));
				SetMaterials(entity.second->materialsIDs);
				Render(_entityHandler->GetMesh()->GetVertexCount(*entity.second));
			}
		}
	}

	//SaveImage("Iron", _backBufferTexture);
	EndScene();
}

void Engine::HandleJobs()
{
	vector<MeshData> meshJobs = _entityHandler->GetMeshJobs();
	for (size_t i = 0; i < meshJobs.size(); i++)
	{
		if (meshJobs[i].nrOfUsers > 0)
		{
			if(!meshJobs[i].update)
				_bufferHandler->CreateMeshBuffer(meshJobs[i]);
			else if (meshJobs[i].update)
				_bufferHandler->UpdateMeshBuffer(meshJobs[i]);
		}
		else
			_bufferHandler->RemoveMeshBuffer(meshJobs[i].identifier);
	}
	_entityHandler->ClearMeshJobs();

	vector<MaterialData> materialJobs = _entityHandler->GetMaterialJobs();
	for (size_t i = 0; i < materialJobs.size(); i++)
	{
		if (materialJobs[i].nrOfUsers > 0)
			_materialHandler->LoadMaterial(materialJobs[i]);
	}
	_entityHandler->ClearMaterialJobs();

	vector<ShaderData> shaderJobs = _entityHandler->GetShaderJobs();
	for (size_t i = 0; i < shaderJobs.size(); i++)
	{
		if (shaderJobs[i].nrOfUsers > 0)
			_shaderHandler->LoadShaders(shaderJobs[i]);
	}
	_entityHandler->ClearShaderJobs();

	//if(_entityHandler->GetLightJobs().size() > 0)
	//	_bufferHandler->UpdateLightBuffer(_entityHandler->GetLightJobs());
	//_entityHandler->ClearLightJobs();


	// Lights
	vector<LightBuffer> lightBuffers;
	for (pair<string, Entity*> entity : _entityHandler->_entities)
	{
		if (entity.second->lightID != -1 && _entityHandler->GetLightData(*entity.second).buffer.active.x == 1)
		{
			lightBuffers.push_back(LightBuffer());
			lightBuffers.back().active.x = 1;
			lightBuffers.back().color = _entityHandler->GetLightData(*entity.second).buffer.color;
			lightBuffers.back().intesity = _entityHandler->GetLightData(*entity.second).buffer.intesity;
			lightBuffers.back().position = _entityHandler->GetTransform()->GetPosition(*entity.second);
		}
	}
	_bufferHandler->UpdateLightBuffer(lightBuffers);
}

Scene * Engine::CreateScene()
{
	_scenes.push_back(new Scene());
	return _scenes.back();
}

LRESULT CALLBACK Engine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		_gui->Create(hWnd);
	break;
	case WM_COMMAND:
		_gui->Command(hWnd, wParam, lParam);
	break;
	case WM_DISPLAYCHANGE:
	{
		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND Engine::GetWindow(){return _window;}

XMFLOAT2 Engine::GetWinDim(){return _winDim;}

void Engine::SetMaterials(EntityMaterials data)
{
	_devcon->PSSetShaderResources(0, 1, _materialHandler->GetMaterial(data.albedo));
	_devcon->PSSetShaderResources(1, 1, _materialHandler->GetMaterial(data.roughness));
	_devcon->PSSetShaderResources(2, 1, _materialHandler->GetMaterial(data.metallic));
	_devcon->PSSetShaderResources(3, 1, _materialHandler->GetMaterial(data.normal));
	_devcon->PSSetShaderResources(4, 1, _materialHandler->GetMaterial(data.displacement));
	_devcon->PSSetShaderResources(5, 1, _materialHandler->GetMaterial(data.radience));
	_devcon->PSSetShaderResources(6, 1, _materialHandler->GetMaterial(data.irradience));
	_devcon->PSSetShaderResources(7, 1, _materialHandler->GetMaterial(data.ambientocclusion));
}

void Engine::SetTransformBuffer(XMFLOAT4X4 matrix)
{
	_bufferHandler->UpdateBuffer(*_bufferHandler->GetTransform(), &XMMatrixTranspose(XMLoadFloat4x4(&matrix)), sizeof(XMFLOAT4X4));
	_devcon->VSSetConstantBuffers(1, 1, _bufferHandler->GetTransform());
}

GUI * Engine::GetGuiHandler(){return _gui;}

TimerClass * Engine::GetTimer(){return _timer;}

void Engine::Update(){}

void Engine::SetActiveCamera(CameraData* data)
{
	_devcon->VSSetConstantBuffers(0, 1, _bufferHandler->GetCamera());
	_devcon->GSSetConstantBuffers(0, 1, _bufferHandler->GetCamera());
	_devcon->PSSetConstantBuffers(2, 1, _bufferHandler->GetCamera());
}

void Engine::SetActiveCameraInfo(CameraData * data){_devcon->PSSetConstantBuffers(1, 1, _bufferHandler->GetCameraInfo());}

void Engine::SetPBRChoice(){_devcon->PSSetConstantBuffers(3, 1, _bufferHandler->GetPBRChoice());}

void Engine::SaveImage(string filename, ID3D11Texture2D * texture)
{
	string cat = "SavedImages/" + filename + ".png";
	ScratchImage image;
	CaptureTexture(_device, _devcon, texture, image);
	const Image* img = image.GetImage(0, 0, 0);
	SaveToWICFile(*img, WIC_FLAGS_NONE, GUID_ContainerFormatPng, wstring(cat.begin(), cat.end()).c_str());
}

void Engine::ShowFPS()
{
	ostringstream outs;
	outs.precision(6);
	outs << L"3D Viewer" << L"    " << L"FPS:  " << _timer->fps() << L"    " << L"Frame Time: " << _timer->frameDeltaTime() << L"  (ms)";
	SetWindowText(_window, outs.str().c_str());
}

void Engine::CreateRasterizers()
{
	// cullback
	_rasterizers.resize(4);
	D3D11_RASTERIZER_DESC descRaster;
	ZeroMemory(&descRaster, sizeof(D3D11_RASTERIZER_DESC));
	descRaster.FillMode = D3D11_FILL_SOLID;					// WIREFRAME;
	descRaster.CullMode = D3D11_CULL_BACK;
	descRaster.MultisampleEnable = TRUE;
	_device->CreateRasterizerState(&descRaster, &_rasterizers[0]);

	// cullfront
	ZeroMemory(&descRaster, sizeof(D3D11_RASTERIZER_DESC));
	descRaster.FillMode = D3D11_FILL_SOLID;					// WIREFRAME;
	descRaster.CullMode = D3D11_CULL_FRONT;
	descRaster.MultisampleEnable = TRUE;
	_device->CreateRasterizerState(&descRaster, &_rasterizers[1]);

	// cullnone
	ZeroMemory(&descRaster, sizeof(D3D11_RASTERIZER_DESC));
	descRaster.FillMode = D3D11_FILL_SOLID;					// WIREFRAME;
	descRaster.CullMode = D3D11_CULL_NONE;
	descRaster.MultisampleEnable = TRUE;
	_device->CreateRasterizerState(&descRaster, &_rasterizers[2]);

	// wireframe
	ZeroMemory(&descRaster, sizeof(D3D11_RASTERIZER_DESC));
	descRaster.FillMode = D3D11_FILL_WIREFRAME;					// WIREFRAME;
	descRaster.CullMode = D3D11_CULL_NONE;
	descRaster.MultisampleEnable = TRUE;
	_device->CreateRasterizerState(&descRaster, &_rasterizers[3]);
}

void Engine::SetRasterizer(RastState state){_devcon->RSSetState(_rasterizers[state]);}

void Engine::RenderGrid()
{
	_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	_devcon->IASetVertexBuffers(0, 1, &_gridBuffer, &_gridStrides, &_gridOffsets);
	_devcon->Draw((unsigned int)_gridData.size(), 0);
}

void Engine::CreateGrid(unsigned int size, float spacing, XMFLOAT3 origo)
{
	float offset = (size * spacing) / 2.0f;
	// X-axis
	for (size_t i = 0; i <= size; i++)
	{
		_gridData.push_back(XMFLOAT3((0.0f - offset) + origo.x, 0.0f + origo.y, (i * spacing - offset) + origo.z));
		_gridData.push_back(XMFLOAT3((size * spacing - offset) + origo.x, 0.0f + origo.y, (i * spacing - offset) + origo.z));
	}
	// Z-axis
	for (size_t i = 0; i <= size; i++)
	{
		_gridData.push_back(XMFLOAT3((i * spacing - offset) + origo.x, 0.0f + origo.y, (0.0f - offset) + origo.z));
		_gridData.push_back(XMFLOAT3((i * spacing - offset) + origo.x, 0.0f + origo.y, (size * spacing - offset) + origo.z));
	}

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(XMFLOAT3) * (unsigned int)_gridData.size();;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = _gridData.data();
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	HRESULT hr = _device->CreateBuffer(&bufferDesc, &subData, &_gridBuffer);
}

void Engine::SetLight(int id)
{
	_devcon->PSSetConstantBuffers(0, 1, _bufferHandler->GetLightBuffers());
}

void Engine::BeginScene()
{
	float clearColor[] = { 0, 0, 0, 1 };
	_devcon->ClearRenderTargetView(_backbuffer, clearColor);
	_devcon->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Engine::EndScene(){_swapChain->Present(0, 0);}

void Engine::Render(unsigned int size)
{
	_devcon->Draw(size, 0);
}