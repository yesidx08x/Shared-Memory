#include "Viewer.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// Opens debug window
	//if (AllocConsole())
	//{
	//	freopen("CONOUT$", "wt", stdout);
	//	SetConsoleTitle(L"Debug Console");
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	//	HWND winHandle = FindWindow(NULL, L"Debug Console");
	//	MoveWindow(winHandle, ScreenWidth, 0, ScreenWidth, ScreenHeight, true);
	//}
	Viewer viewer(hInstance, nCmdShow);
	return viewer.Run();
}