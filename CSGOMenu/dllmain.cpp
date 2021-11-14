#include <Windows.h>
#include <iostream>

#include "Aimbot.h"

DWORD WINAPI OnDllAttach(PVOID base) {
#ifdef _DEBUG
	AllocConsole();
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"Aimbot - DEBUG");
#endif // _DEBUG


	while (!(GetAsyncKeyState(VK_DELETE) & 0x8000)) {
		if(GetAsyncKeyState(VK_MENU))
			Run();
		//Sleep(1);
	}
	FreeLibraryAndExitThread(static_cast<HMODULE>(base), 0);
}

VOID WINAPI OnDllDetach() {
#ifdef _DEBUG
	fclose((FILE*)stdin);
	fclose((FILE*)stdout);

	HWND hw_ConsoleHwnd = GetConsoleWindow();
	FreeConsole();
	PostMessageW(hw_ConsoleHwnd, WM_CLOSE, 0, 0);
#endif // _DEBUG
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD reason, LPVOID lpRevered) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OnDllAttach, hModule, NULL, NULL);
	}
	else if (reason == DLL_PROCESS_DETACH) {
		OnDllDetach();
	}

	return TRUE;
}