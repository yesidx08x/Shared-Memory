#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include "Viewer.h"
#ifdef _DEBUG
#include <crtdbg.h>
#include <vld.h>
#endif

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// Opens up a cmd window where all prints get to
#ifdef _DEBUG
	FILE* stream;
	errno_t err;
	if (AllocConsole())
	{
		err = freopen_s(&stream, "CONOUT$", "wt", stdout);
		SetConsoleTitleW(L"Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		HWND winHandle = FindWindowW(NULL, L"Debug Console");
		MoveWindow(winHandle, (int)ScreenWidth, 0, (int)ScreenWidth, (int)ScreenHeight, true);
	}
#endif
	Viewer viewer(hInstance);
	int result = viewer.Run(nCmdShow);
#ifdef _DEBUG
	if(err == 0)
		fclose(stream);
#endif
	return result;
}