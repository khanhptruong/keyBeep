#include <Windows.h>
#include <iostream>

LRESULT CALLBACK KBDHook(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* s = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	switch (wParam) {
	case WM_KEYDOWN:
		char c = MapVirtualKey(s->vkCode, MAPVK_VK_TO_CHAR);
		//std::cout << c << std::endl;
		Beep(300, 100);
		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
	std::cout << "Key Beep Start!" << std::endl;

	HHOOK kbd = SetWindowsHookEx(WH_KEYBOARD_LL, &KBDHook, 0, 0);

	//win32 api message pump
	//its like game loop... kinda...
	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL) > 0) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(kbd);

	return 0;
}