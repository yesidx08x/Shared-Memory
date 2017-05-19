#pragma once
#include "Engine.h"
#include <sstream>
#include "FBXConverter.h"
#include "../Receiver/Inc/MDataHandler.h"

class Viewer
{
public:
	Viewer();
	Viewer(HINSTANCE hInstance, int nCmdShow);
	~Viewer();
	int Run();

private:
	Engine* _engine;
	Scene* _scene;
	HWND _wndHandle;
	int _nCmdShow;
	DWORD _deltaTime;
	DWORD _startTime;
	FBXConverter* _fbxConverter;
	EntityHandler* _entityHandler;
	MDataHandler* _dataHandler;

	void PopulateScene();
	void Update();

	float rot = 0.0f;
	EntityHandler::EntityAmounts* _entityAmounts;
	map<string, Entity*> _mayaEntites;
	//vector<Entity*> _mayaEntites;
};