#include "GUI.h"

bool GUI::_openShared;
HWND GUI::_shareButton;

GUI::GUI(){}

GUI::~GUI(){}

void GUI::Create(HWND hwnd)
{
	HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);

	// Button to start shared memory
	_shareButton = CreateWindowExW(NULL,
		L"BUTTON",
		L"Start Shared",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		0,
		0,
		100,
		24,
		hwnd,
		(HMENU)0,
		GetModuleHandleW(NULL),
		NULL);

	SendMessageW(_shareButton, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
}

void GUI::Command(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if (!_openShared)
	{
		SetWindowTextW(_shareButton, L"Pause Sharing");
		_openShared = true;
	}
	else
	{
		SetWindowTextW(_shareButton, L"Resume Sharing");
		_openShared = false;
	}
}

bool GUI::IsSharedOpen(){return _openShared;}