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
#include <conio.h>

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
bool InjectDLL(DWORD process_id_, const wchar_t *dll_file_);

int wmain(int argc, wchar_t **argv) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: [dll_path] [executable]\n";
      return EXIT_FAILURE;
    }

    // Get process handle
    std::wstring dll_path = argv[1], exe_path = argv[2];

    DWORD pid = GetProcId(exe_path.c_str());
    if (!pid) HandleError(L"Couldn't get pid");

    std::cout << "Trying to inject dll...\n";
    if (!InjectDLL(pid, dll_path.c_str())) HandleError(L"Couldn't inject dll");
    std::cout << "Success!\nPress any key to exit...\n";
    int temp = _getch();
    
  } catch (...) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void HandleError(std::wstring_view err) {
  std::wstringstream ws;
  ws << "Error: " << err << "\n"
     << "WinAPI: " << GetLastError() << ": " << GetErrorMessage(GetLastError());
  MessageBox(NULL, ws.str().c_str(), L"Error!", MB_OK);
  throw std::runtime_error("an error occured");
}

DWORD GetProcId(const wchar_t *procName) {
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
bool InjectDLL(DWORD process_id_, const wchar_t *dll_file_) {
  // get the full path of the dll file
  TCHAR full_dll_path[MAX_PATH];
  GetFullPathName(dll_file_, MAX_PATH, full_dll_path, NULL);

  // get the function LoadLibraryA
  LPVOID load_library =
      GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
  if (load_library == NULL) {
    return false;
  }

  // open the process
  HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, false, process_id_);
  if (process_handle == NULL) {
    return false;
  }

  // allocate space to write the dll location
  LPVOID dll_parameter_address =
      VirtualAllocEx(process_handle, 0, wcslen(full_dll_path),
                     MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  if (dll_parameter_address == NULL) {
    CloseHandle(process_handle);
    return false;
  }

  // write the dll location to the space we previously allocated
  BOOL wrote_memory =
      WriteProcessMemory(process_handle, dll_parameter_address, full_dll_path,
                         wcslen(full_dll_path), NULL);
  if (wrote_memory == false) {
    CloseHandle(process_handle);
    return false;
  }

  // launch the dll using LoadLibraryA
  HANDLE dll_thread_handle = CreateRemoteThread(
      process_handle, 0, 0, (LPTHREAD_START_ROUTINE)load_library,
      dll_parameter_address, 0, 0);
  if (dll_thread_handle == NULL) {
    CloseHandle(process_handle);
    return false;
  }

  CloseHandle(dll_thread_handle);
  CloseHandle(process_handle);
  return true;
}