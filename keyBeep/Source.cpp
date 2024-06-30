#include <Windows.h>
#include <iostream>
#include <WinUser.h>

enum KeyEdge { UP, DOWN };

void replaceKey(DWORD replacementKey, WORD scanKey, KeyEdge kEdge)
{
	INPUT inputs[1] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = replacementKey;
	inputs[0].ki.wScan = scanKey;
	//if kEdge is DOWN, don't set dwFlag
	if (kEdge == UP) { inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; }

	UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	if (uSent != ARRAYSIZE(inputs)) { std::cout << "SendInput failed: 0x%x" << std::endl; }
}

LRESULT CALLBACK KBDHook(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* s = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
	KeyEdge kEdge;

	if (wParam == WM_KEYDOWN)    { kEdge = DOWN; }
	else if (wParam == WM_KEYUP) { kEdge = UP; }
	else						 { return CallNextHookEx(NULL, nCode, wParam, lParam); }

	switch (s->vkCode)
	{
	case 0x57: //W
		replaceKey(VK_UP, 72, kEdge);
		return 1; //if key is replaced, dont forward original key along
	case 0x41: //A
		replaceKey(VK_LEFT, 75, kEdge);
		return 1; //if key is replaced, dont forward original key along
	case 0x53: //S
		replaceKey(VK_DOWN, 80, kEdge);
		return 1; //if key is replaced, dont forward original key along
	case 0x44: //D
		replaceKey(VK_RIGHT, 77, kEdge);
		return 1; //if key is replaced, dont forward original key along
	default: //all other keys, forward along as usual
		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main() {
	std::cout << "Key Replace Start!" << std::endl;

	std::cout << "Creating hook..." << std::endl;
	HHOOK kbd = SetWindowsHookEx(WH_KEYBOARD_LL, &KBDHook, 0, 0);

	//win32 api message pump
	//its like game loop... kinda...
	std::cout << "Message pump looping..." << std::endl;
	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL) > 0) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	std::cout << "Cleaning up hook..." << std::endl;
	UnhookWindowsHookEx(kbd);

	return 0;
}