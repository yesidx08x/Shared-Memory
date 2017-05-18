#include "GUI.h"

XMINT4 GUI::_pbrChoice;
bool GUI::_openShared;
HWND GUI::_shareButton;

GUI::GUI(){}

GUI::~GUI(){}

int* GUI::GetPBRChoice(){return &_pbrChoice.x;}

void GUI::Create(HWND hwnd)
{
	_pbrChoice = XMINT4(1, 0, 0, 0);
	HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);

	//HWND frostbiteButton = CreateWindowEx(NULL,
	//	L"BUTTON",
	//	L"Frostbite",
	//	WS_TABSTOP | WS_VISIBLE |
	//	WS_CHILD | BS_DEFPUSHBUTTON,
	//	0,
	//	0,
	//	100,
	//	24,
	//	hwnd,
	//	(HMENU)frostbite,
	//	GetModuleHandle(NULL),
	//	NULL);

	//SendMessage(frostbiteButton, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

	//HWND cookTorranceButton = CreateWindowEx(NULL,
	//	L"BUTTON",
	//	L"Cook-Torrance",
	//	WS_TABSTOP | WS_VISIBLE |
	//	WS_CHILD | BS_DEFPUSHBUTTON,
	//	0,
	//	24,
	//	100,
	//	24,
	//	hwnd,
	//	(HMENU)cookTorrance,
	//	GetModuleHandle(NULL),
	//	NULL);

	//SendMessage(cookTorranceButton, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

	//HWND ue4Button = CreateWindowEx(NULL,
	//	L"BUTTON",
	//	L"Unreal Engine 4",
	//	WS_TABSTOP | WS_VISIBLE |
	//	WS_CHILD | BS_DEFPUSHBUTTON,
	//	0,
	//	48,
	//	100,
	//	24,
	//	hwnd,
	//	(HMENU)ue4,
	//	GetModuleHandle(NULL),
	//	NULL);

	//SendMessage(ue4Button, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

	_shareButton = CreateWindowEx(NULL,
		"BUTTON",
		"Start Shared",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		0,
		0,
		100,
		24,
		hwnd,
		(HMENU)0,
		GetModuleHandle(NULL),
		NULL);

	SendMessage(_shareButton, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
}

void GUI::Command(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	//_pbrChoice.x = int(wParam);

	if (!_openShared)
	{
		SetWindowText(_shareButton, "Pause Sharing");
		_openShared = true;
	}
	else
	{
		SetWindowText(_shareButton, "Resume Sharing");
		_openShared = false;
	}
}

bool GUI::IsSharedOpen(){return _openShared;}