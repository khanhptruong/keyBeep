#include <Windows.h>
#include <iostream>

LRESULT CALLBACK KBDHook(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* s = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	switch (wParam) {
	case WM_KEYDOWN:
		//char c = MapVirtualKey(s->vkCode, MAPVK_VK_TO_CHAR);

		/*
		Note for PlaySound:

		Need to link winmm.lib library to use PlaySound.
		Righ Click Project > properties > linker > input < add ;winmm.lib at the end

		https://stackoverflow.com/questions/21034935/playsound-in-c
		*/
		if (s->vkCode == VK_BACK)
			PlaySound(L"C:\\Users\\khanh\\source\\repos\\keyBeep\\keyBeep\\Keyboard-Delete.wav", NULL, SND_FILENAME | SND_ASYNC);
		else
			PlaySound(L"C:\\Users\\khanh\\source\\repos\\keyBeep\\keyBeep\\Keyboard-Key.wav", NULL, SND_FILENAME | SND_ASYNC);

		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main() {
	std::cout << "Key Beep Start!" << std::endl;

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