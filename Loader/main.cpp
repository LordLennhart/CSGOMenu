#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

typedef std::basic_string<TCHAR> String;
typedef void(__stdcall *unload_function_t)();

String GetErrorMessage(DWORD dwErrorCode) {
  LPTSTR psz = NULL;
  const DWORD cchMsg =
      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
                        FORMAT_MESSAGE_ALLOCATE_BUFFER,
                    NULL,  // (not used with FORMAT_MESSAGE_FROM_SYSTEM)
                    dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    reinterpret_cast<LPTSTR>(&psz), 0, NULL);
  if (cchMsg > 0) {
    // Assign buffer to smart pointer with custom deleter so that memory gets
    // released in case String's c'tor throws an exception.
    auto deleter = [](void *p) { ::HeapFree(::GetProcessHeap(), 0, p); };
    std::unique_ptr<TCHAR, decltype(deleter)> ptrBuffer(psz, deleter);
    return String(ptrBuffer.get(), cchMsg);
  } else {
    throw std::runtime_error("Failed to retrieve error message string.");
  }
}
void HandleError(std::wstring_view err);
DWORD GetProcId(const wchar_t *procName);

int wmain(int argc, wchar_t** argv) {
  HANDLE process_handle = NULL, remote_thread = NULL;
  HMODULE lib = NULL;
  unload_function_t unload_func_ptr = NULL;

  try {
    if (argc != 3) {
      
      std::cerr << "Usage: [dll_path] [executable]\n";
      return EXIT_FAILURE;
    }

    // Get process handle
    std::wstring dll_path = argv[2], exe_path = argv[1];

    DWORD pid = GetProcId(exe_path.c_str());
    if (!pid) HandleError(L"Couldn't get pid");
    process_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (!process_handle) HandleError(L"Couldn't open handle to process");

    // load library
    lib = LoadLibrary(dll_path.c_str());
    if (!lib) HandleError(L"Couldn't load specified library");

    // Load functions

    unload_func_ptr = (unload_function_t)GetProcAddress(lib, "Unload");
    LPTHREAD_START_ROUTINE entry_func_ptr =
        (LPTHREAD_START_ROUTINE)GetProcAddress(lib, "Load");

    if (!unload_func_ptr || !entry_func_ptr)
      HandleError(L"Couldn't load functions");

    // Launch dll
    remote_thread = CreateRemoteThread(process_handle, NULL, 0, entry_func_ptr,
                                       NULL, 0, NULL);
    if (!remote_thread) HandleError(L"Couldn't launch remote thread");

    MSG messages;
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0)) {
      /* Translate virtual-key messages into character messages */
      TranslateMessage(&messages);

      if (messages.message == WM_KEYDOWN) {
        if (messages.wParam == VK_ESCAPE) {
          break;
        }
      }

      /* Send message to WindowProcedure */
      DispatchMessage(&messages);
    }
  } catch (...) {
    if (lib) {
      if (remote_thread) {
        if (unload_func_ptr) {
          unload_func_ptr();
        }
        CloseHandle(remote_thread);
      }
      FreeLibrary(lib);
    }
    if (process_handle) {
      CloseHandle(process_handle);
    }
    return EXIT_FAILURE;
  }

  if (unload_func_ptr) {
    unload_func_ptr();
  }
  CloseHandle(remote_thread);
  FreeLibrary(lib);
  CloseHandle(process_handle);
  return EXIT_SUCCESS;
}

void HandleError(std::wstring_view err) {
  std::wstringstream ws;
  ws << "Error: " << err << "\n"
     << "WinAPI: " << GetLastError() << ": " << GetErrorMessage(GetLastError());
  MessageBox(NULL, ws.str().c_str(), L"Error!", MB_OK);
  throw std::runtime_error("an error occured");
}

DWORD
GetProcId(const wchar_t *procName) {
  DWORD procId = 0;
  HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnap != INVALID_HANDLE_VALUE) {
    PROCESSENTRY32 procEntry{};
    procEntry.dwSize = sizeof(procEntry);

    if (Process32First(hSnap, &procEntry)) {
      do {
        if (!_wcsicmp(procEntry.szExeFile, procName)) {
          procId = procEntry.th32ProcessID;
          break;
        }
      } while (Process32Next(hSnap, &procEntry));
    }
  }
  CloseHandle(hSnap);
  return procId;
}