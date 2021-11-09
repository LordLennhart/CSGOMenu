#include <Windows.h>
#include <iostream>

#include "Aimbot.h"

DWORD WINAPI OnDllAttach(PVOID base) {
  while (!GetAsyncKeyState(VK_DELETE) & 0x8000) {
    Run();
    Sleep(1);
  }
  FreeLibraryAndExitThread(static_cast<HMODULE>(base), 0);
}

VOID WINAPI OnDllDetach() {
//#ifdef _DEBUG
//  HWND hw_ConsoleHwnd = GetConsoleWindow();
//  PostMessageW(hw_ConsoleHwnd, WM_CLOSE, 0, 0);
//#endif  // _DEBUG
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  if (ul_reason_for_call = DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(hModule);
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OnDllAttach, hModule, NULL,
                 NULL);
  } else if (ul_reason_for_call = DLL_PROCESS_DETACH) {
    OnDllDetach();
  }

  /* switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:

      __fallthrough;
    case DLL_PROCESS_DETACH:
      MessageBeep(MB_OK);
      break;

    case DLL_THREAD_ATTACH:
      __fallthrough;
    case DLL_THREAD_DETACH:
      MessageBeep(MB_OK);
      break;
  }
  Beep(100, 2000); */
  return TRUE;
}
