#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

#include "Aimbot.h"

std::thread *g_aimbot_thr = nullptr;
std::atomic_bool g_should_quit = false;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_THREAD_ATTACH:
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
  }
  return TRUE;
}
