#define _DEBUG 
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Viewer.h"
#include <vld.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	//_CrtSetBreakAlloc(206);
	// Opens debug window
	if (AllocConsole())
	{
		freopen("CONOUT$", "wt", stdout);
		SetConsoleTitle("Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		HWND winHandle = FindWindow(NULL, "Debug Console");
		MoveWindow(winHandle, ScreenWidth, 0, ScreenWidth, ScreenHeight, true);
	}
#endif

	Viewer viewer(hInstance, nCmdShow);
	int result = viewer.Run();
	//struct AtExit
	//{
	//	~AtExit() { _CrtDumpMemoryLeaks(); }
	//} doAtExit;
	return result;
}