#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

#include "Aimbot.h"

DWORD WINAPI OnDllAttach(PVOID base) {
#ifdef _DEBUG
  AllocConsole();
  freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
  freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
  SetConsoleTitleA("BLA - Debug");
#endif  // _DEBUG

  while (!GetAsyncKeyState(VK_DELETE) & 0x8000) {
    Run();
    Sleep(1);
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
#endif  // _DEBUG
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(hModule);
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OnDllAttach, hModule, NULL,
                 NULL);
  } else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
    OnDllDetach();
  }

  /*switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      __fallthrough;
    case DLL_PROCESS_ATTACH:
      if (!g_aimbot_thr) {
        g_aimbot_thr = new std::thread([]() {
          while (!g_should_quit.load()) {
            Run();
          }
        });
      }
      break;

    case DLL_PROCESS_DETACH:
      __fallthrough;
    case DLL_THREAD_DETACH:
      if (g_aimbot_thr) {
        g_should_quit.store(true);
        if (g_aimbot_thr->joinable()) {
          g_aimbot_thr->join();
        } else {
          g_aimbot_thr->detach();
        }
        delete g_aimbot_thr;
        g_aimbot_thr = nullptr;
      }
      break;
  }*/
  // Beep(100, 2000);
  return TRUE;
}
