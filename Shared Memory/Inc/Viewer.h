#pragma once
#include "Engine.h"
#include <sstream>
#include "FBXConverter.h"
#include "../Receiver/Inc/MDataHandler.h"

class Viewer
{
public:
	Viewer();
	Viewer(HINSTANCE hInstance);
	~Viewer();
	int Run(int nCmdShow);

private:
	Engine* _engine = nullptr;
	Scene* _scene = nullptr;
	HWND _wndHandle;
	DWORD _deltaTime = 0;
	DWORD _startTime = 0;
	FBXConverter* _fbxConverter = nullptr;
	EntityHandler* _entityHandler = nullptr;
	MDataHandler* _dataHandler = nullptr;

	void PopulateScene();
	void Update();

	float rot = 0.0f;
	map<string, Entity*> _mayaEntites;
};