#include <Windows.h>
#include <iostream>
#include <csignal>
//#include <fileapi.h>

#define LOCK_WAV L"C:\\Users\\khanh\\portable\\khanh\\lock.wav"
#define KB_DEL_WAV L"C:\\Users\\khanh\\portable\\khanh\\Keyboard-Delete.wav"
#define KB_KEY_WAV L"C:\\Users\\khanh\\portable\\khanh\\Keyboard-Key.wav"

HHOOK kbd; //needs to be global to use in atexit

bool isFileExist(LPCWSTR filePath)
{
	DWORD fileAttrib = GetFileAttributesW(filePath);
	if (fileAttrib == INVALID_FILE_ATTRIBUTES) return false;
	else									   return true;
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
		case VK_RETURN:
		case VK_ESCAPE:
			PlaySound(LOCK_WAV, NULL, SND_FILENAME | SND_ASYNC);
			break;
		case VK_BACK:
		case VK_DELETE:
			PlaySound(KB_DEL_WAV, NULL, SND_FILENAME | SND_ASYNC);
			break;
		case VK_LCONTROL:
		case VK_RCONTROL:
		case VK_LSHIFT:
		case VK_RSHIFT:
		case VK_LWIN:
		case VK_RWIN:
			//no sound
			break;
		default:
			PlaySound(KB_KEY_WAV, NULL, SND_FILENAME | SND_ASYNC);
			break;
		}
	}
		

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void unhookAtExit(int sigNum) {
	std::cout << "Cleaning up hook..." << std::endl;
	UnhookWindowsHookEx(kbd);
	std::cout << "Done." << std::endl;
	exit(sigNum);
}

int main() {
	if (isFileExist(LOCK_WAV) && isFileExist(KB_DEL_WAV) && isFileExist(KB_KEY_WAV))
	{
		std::cout << "All sound files exist." << std::endl;
	}
	else 
	{
		std::cout << "One or more sound files missing." << std::endl;
		return 1;
	}

	std::cout << "Key Beep Start!" << std::endl;

	signal(SIGINT, unhookAtExit);

	std::cout << "Creating hook..." << std::endl;
	kbd = SetWindowsHookEx(WH_KEYBOARD_LL, &KBDHook, 0, 0);

	//win32 api message pump
	//its like game loop... kinda...
	std::cout << "Message pump looping..." << std::endl;
	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL) > 0) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}