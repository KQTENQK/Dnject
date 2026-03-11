#ifndef __INJECTOR__
#define __INJECTOR__

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <variant>

namespace Dnject
{
    class Injector
    {
        private:
            using ErrorMessage = std::wstring;

        public:
            DWORD GetProcessIdByName(const std::wstring& processName)
            {
                DWORD pid = 0;
                HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

                if (snap == INVALID_HANDLE_VALUE)
                    return 0;

                PROCESSENTRY32W entry;
                entry.dwSize = sizeof(entry);

                if (Process32FirstW(snap, &entry))
                {
                    do
                    {
                        if (_wcsicmp(entry.szExeFile, processName.c_str()) == 0)
                        {
                            pid = entry.th32ProcessID;

                            break;
                        }
                    } while (Process32NextW(snap, &entry));
                }

                CloseHandle(snap);

                return pid;
            }

            std::variant<bool, ErrorMessage> Inject
            (
                const std::wstring& processName,
                const std::wstring& dllPath
            )
            {
                DWORD processId = GetProcessIdByName(processName);

                if (processId == 0)
                    return L"Process not found.\n";

                return Inject(processId, dllPath);
            }

            std::variant<bool, ErrorMessage> Inject
            (
                DWORD processId,
                const std::wstring& dllPath
            )
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

                if (!hProcess)
                    return L"OpenProcess failed. Error: " + std::to_wstring(GetLastError());

                size_t dllPathSize = (wcslen(dllPath.c_str()) + 1) * sizeof(wchar_t);

                LPVOID pRemoteMem = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT, PAGE_READWRITE);

                if (!pRemoteMem)
                {
                    CloseHandle(hProcess);

                    return L"VirtualAllocEx failed. Error: " + std::to_wstring(GetLastError());
                }

                if (!WriteProcessMemory(hProcess, pRemoteMem, dllPath.c_str(), dllPathSize, NULL))
                {
                    VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
                    CloseHandle(hProcess);

                    return L"WriteProcessMemory failed. Error: " + std::to_wstring(GetLastError());
                }

                LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");
                HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteMem, 0, NULL);

                if (!hThread)
                {
                    VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
                    CloseHandle(hProcess);

                    return L"CreateRemoteThread failed. Error: " + std::to_wstring(GetLastError());
                }

                WaitForSingleObject(hThread, INFINITE);
                CloseHandle(hThread);
                VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
                CloseHandle(hProcess);

                return true;
            }
    };
}

#endif
