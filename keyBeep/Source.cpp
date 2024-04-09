#include <Windows.h>
#include <iostream>

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
		case VK_RETURN:
		case VK_ESCAPE:
			PlaySound(L"C:\\Users\\khanh\\source\\repos\\keyBeep\\keyBeep\\lock.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
		case VK_BACK:
		case VK_DELETE:
			PlaySound(L"C:\\Users\\khanh\\source\\repos\\keyBeep\\keyBeep\\Keyboard-Delete.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
		case VK_LCONTROL:
		case VK_RCONTROL:
		case VK_LSHIFT:
		case VK_RSHIFT:
			//no sound
			break;
		default:
			PlaySound(L"C:\\Users\\khanh\\source\\repos\\keyBeep\\keyBeep\\Keyboard-Key.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
		}
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