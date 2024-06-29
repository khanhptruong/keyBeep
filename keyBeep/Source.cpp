#include <Windows.h>
#include <iostream>
#include <WinUser.h>

void replaceKey(DWORD replacementKey)
{
	INPUT inputs[2] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = replacementKey;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = replacementKey;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;


	UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	if (uSent != ARRAYSIZE(inputs))
	{
		std::cout << "SendInput failed: 0x%x" << std::endl;
	}
}

LRESULT CALLBACK KBDHook(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* s = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	if (wParam == WM_KEYDOWN)
	{
		//char c = MapVirtualKey(s->vkCode, MAPVK_VK_TO_CHAR);

		/*
		Note for PlaySound:

		Need to link winmm.lib library to use PlaySound.
		Righ Click Project > properties > linker > input < add ;winmm.lib at the end

		https://stackoverflow.com/questions/21034935/playsound-in-c
		*/

		switch (s->vkCode) {
		case 0x57: //W
			replaceKey(VK_UP);
			return 1; //if key is replaced, dont forward original key along
			break;
		case 0x41: //A
			replaceKey(VK_LEFT);
			return 1; //if key is replaced, dont forward original key along
			break;
		case 0x53: //S
			replaceKey(VK_DOWN);
			return 1; //if key is replaced, dont forward original key along
			break;
		case 0x44: //D
			replaceKey(VK_RIGHT);
			return 1; //if key is replaced, dont forward original key along
			break;
		default: //all other keys, forward along as usual
			break;
		}
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