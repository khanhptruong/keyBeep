#include <Windows.h>
#include <iostream>

int keyCount = 0;

DWORD WINAPI beepThreadFunction(LPVOID lpParam) {
	//std::cout << "." << std::endl;
	while (keyCount != -1) {
		if (keyCount > 3)
			keyCount = 2;
		else if (keyCount > 0)
		{
			Beep(300, 90);
			keyCount--;
		}
	}
	return 1;
}

LRESULT CALLBACK KBDHook(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* s = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	switch (wParam) {
	case WM_KEYDOWN:
		//char c = MapVirtualKey(s->vkCode, MAPVK_VK_TO_CHAR);
		keyCount++;
		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main() {
	std::cout << "Key Beep Start!" << std::endl;

	std::cout << "Creating hook..." << std::endl;
	HHOOK kbd = SetWindowsHookEx(WH_KEYBOARD_LL, &KBDHook, 0, 0);

	std::cout << "Creating Beep Thread" << std::endl;
	CreateThread(NULL, 0, beepThreadFunction, NULL, 0, NULL);

	//win32 api message pump
	//its like game loop... kinda...
	std::cout << "Message pump looping..." << std::endl;
	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL) > 0) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	std::cout << "Cleaning up hook and thread..." << std::endl;
	UnhookWindowsHookEx(kbd);
	keyCount = -1;

	return 0;
}