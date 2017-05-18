#pragma once
#include <vector>
#include <windows.h>
#include <iostream>

using namespace std;

enum KeyState{pressed, down, up, released, KeyStateAmount};
enum Keys{A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, space, LMB, RMB};

class InputHandler
{
private:
	struct Key
	{
		int id;
		KeyState state = released;
	};
	vector<Key> _keys;

public:
	InputHandler();
	~InputHandler();
	bool CheckInput(Keys key, KeyState state);
};