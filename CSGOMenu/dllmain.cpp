#include <Windows.h>
#include <iostream>

#include "Aimbot.h"
#include "pch.h"

DWORD WINAPI OnDllAttach(PVOID base) {
#ifdef _DEBUG
    AllocConsole();
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    SetConsoleTitleA("CSGOMenu - Debug");
#endif // _DEBUG

    while (!(GetAsyncKeyState(VK_DELETE) & 0x8000)) {
        Run();
        Sleep(1);
    }

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

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{   

    if (ul_reason_for_call = DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OnDllAttach, hModule, NULL, NULL);
    }
    else if (ul_reason_for_call = DLL_PROCESS_DETACH)
        OnDllDetach();

   /* switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    } */
    return TRUE;
}
