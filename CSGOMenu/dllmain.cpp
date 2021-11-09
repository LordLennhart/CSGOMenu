#include <Windows.h>
#include <iostream>

#include "Aimbot.h"

extern "C" {
DWORD WINAPI Load(_In_ LPVOID lpParameter) {
#ifdef _DEBUG
  MessageBox(NULL, L"load has been called", L"Lol", MB_OK);
#endif  // _DEBUG

  // while (!(GetAsyncKeyState(VK_DELETE) & 0x8000)) {
  //    Run();
  //    Sleep(1);
  //}
  return EXIT_SUCCESS;
}

void __stdcall Unload() {
#ifdef _DEBUG
  MessageBox(NULL, L"Unload has been called", L"Lol", MB_OK);
#endif  // _DEBUG
}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  return TRUE;
}
