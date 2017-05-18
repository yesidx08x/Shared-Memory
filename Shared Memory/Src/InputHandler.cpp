#include "InputHandler.h"

InputHandler::InputHandler()
{
	_keys.resize(29);

	// A - Z
	_keys[0].id = 65;
	_keys[1].id = 66;
	_keys[2].id = 67;
	_keys[3].id = 68;
	_keys[4].id = 69;
	_keys[5].id = 70;
	_keys[6].id = 71;
	_keys[7].id = 72;
	_keys[8].id = 73;
	_keys[9].id = 74;
	_keys[10].id = 75;
	_keys[11].id = 76;
	_keys[12].id = 77;
	_keys[13].id = 78;
	_keys[14].id = 79;
	_keys[15].id = 80;
	_keys[16].id = 81;
	_keys[17].id = 82;
	_keys[18].id = 83;
	_keys[19].id = 84;
	_keys[20].id = 85;
	_keys[21].id = 86;
	_keys[22].id = 87;
	_keys[23].id = 88;
	_keys[24].id = 89;
	_keys[25].id = 90;

	// Specials
	_keys[26].id = VK_SPACE;
	_keys[27].id = VK_LBUTTON;
	_keys[28].id = VK_RBUTTON;
}

InputHandler::~InputHandler(){}

bool InputHandler::CheckInput(Keys key, KeyState state)
{
	if (GetAsyncKeyState(_keys[key].id))
	{
		if (_keys[key].state == pressed)
			_keys[key].state = down;
		else if(_keys[key].state == released)
			_keys[key].state = pressed;
	}
	else
	{
		if (_keys[key].state == down)
			_keys[key].state = up;
		else
			_keys[key].state = released;
	}

	if (_keys[key].state == state)
		return true;
	else
		return false;
}