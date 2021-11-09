#include <Windows.h>
#include <iostream>

#include "Aimbot.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
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
  Beep(100, 2000);
  return TRUE;
}
