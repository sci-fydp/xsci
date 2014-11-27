#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <string>

using namespace std;

#define DEFAULT_SUBLIME_LOCATION "C:/Program Files/Sublime Text 2/sublime_text.exe"

HWND sublimeWindow;
HWND consoleWindow;

void SetForegroundWindowInternal(HWND hWnd)
{
	if (!::IsWindow(hWnd)) return;

	BYTE keyState[256] = { 0 };
	//to unlock SetForegroundWindow we need to imitate Alt pressing
	if (::GetKeyboardState((LPBYTE)&keyState))
	{
		if (!(keyState[VK_MENU] & 0x80))
		{
			::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
		}
	}

	::SetForegroundWindow(hWnd);

	if (::GetKeyboardState((LPBYTE)&keyState))
	{
		if (!(keyState[VK_MENU] & 0x80))
		{
			::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}
}


void NewSetForegroundWindow(HWND hWnd)
{
	if (!::IsWindow(hWnd)) return;

	BYTE keyState[256] = { 0 };
	//to unlock SetForegroundWindow we need to imitate Alt pressing
	if (::GetKeyboardState((LPBYTE)&keyState))
	{
		if (!(keyState[VK_MENU] & 0x80))
		{
			::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
		}
	}

	::SetForegroundWindow(hWnd);

	if (::GetKeyboardState((LPBYTE)&keyState))
	{
		if (!(keyState[VK_MENU] & 0x80))
		{
			::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}
}

void getSublimeControl()
{
	SetForegroundWindowInternal(sublimeWindow);
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

void getConsoleControl()
{
	
	int ans = SetForegroundWindow(consoleWindow);
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

string acceptConsoleInput()
{
	getConsoleControl();
	string str;
	getline(cin , str);
	return str;
}

void openSublimeConsole()
{
	INPUT input[1];
	memset(input, 0, sizeof(input));

	input[0].type = INPUT_KEYBOARD;
	// Press the "Ctrl" key
	input[0].ki.wVk = VK_CONTROL;
	input[0].ki.dwFlags = 0; // 0 for key press
	SendInput(1, input, sizeof(INPUT));

	// Press the "V" key
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_OEM_3;
	input[0].ki.dwFlags = 0; // 0 for key press
	SendInput(1, input, sizeof(INPUT));


	// Release the "V" key
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_OEM_3;
	input[0].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, input, sizeof(INPUT));


	// Release the "Ctrl" key
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_CONTROL;
	input[0].ki.dwFlags = KEYEVENTF_KEYUP ;
	SendInput(1, input, sizeof(INPUT));

}

void sendText(string str)
{
	getSublimeControl();
	INPUT input[1];
	//memset(input, 0, sizeof(input));
	for (unsigned int i = 0; i < str.length(); i++)
	{
		char c = str.at(i);
		/*
		memset(input, 0, sizeof(input));
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VkKeyScan(c);
		SendInput(1, input, sizeof(INPUT));

		memset(input, 0, sizeof(input));
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VkKeyScan(c);
		input[0].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, input, sizeof(INPUT));
		*/

		memset(input, 0, sizeof(input));
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wScan = c;
		input[0].ki.dwFlags = KEYEVENTF_UNICODE;
		SendInput(1, input, sizeof(INPUT));

		memset(input, 0, sizeof(input));
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wScan = c;
		input[0].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
		SendInput(1, input, sizeof(INPUT));

	}
	memset(input, 0, sizeof(input));
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_RETURN;
	SendInput(1, input, sizeof(INPUT));

	memset(input, 0, sizeof(input));
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_RETURN;
	input[0].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, input, sizeof(INPUT));
}

bool openSublime(string sublimeLocation)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	// Start the child process. 
	LPSTR str = const_cast<char *>(sublimeLocation.c_str());
	std::wstring widestr = std::wstring(sublimeLocation.begin(), sublimeLocation.end());
	LPTSTR szCmdline = _tcsdup(widestr.c_str());
	bool result = CreateProcess(NULL,   // No module name (use command line)
		szCmdline,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return result;
}


int main(int argc, char **argv)
{
	string SUBLIME_LOCATION = DEFAULT_SUBLIME_LOCATION;

	if (argc > 1)
	{
		SUBLIME_LOCATION = argv[1];
	}

	consoleWindow = GetConsoleWindow();
	openSublime(SUBLIME_LOCATION);
	sublimeWindow = GetForegroundWindow();
	cout << sublimeWindow << endl;
	openSublimeConsole();

	
	sendText("hello world.");
	while (true)
	{
		string input = acceptConsoleInput();
		sendText(input);
	}

	return 0;
}