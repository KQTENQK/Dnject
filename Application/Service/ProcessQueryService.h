#ifndef __PROCESSQUERYSERVICE__
#define __PROCESSQUERYSERVICE__

#include <vector>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include "Unii/Unii.h"
#include "Data/ProcessItem.h"

namespace Dnject
{
    class ProcessQueryService : public Unii::IService
    {
        public:
            std::vector<ProcessItem> GetProcesses() const
            {
                std::vector<ProcessItem> result;

                HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

                if (snapshot == INVALID_HANDLE_VALUE)
                    return result;

                PROCESSENTRY32W entry;
                entry.dwSize = sizeof(entry);

                if (Process32FirstW(snapshot, &entry))
                {
                    do
                    {
                        ProcessItem item;

                        item.Id = entry.th32ProcessID;
                        item.Name = entry.szExeFile;

                        result.push_back(item);
                    }
                    while (Process32NextW(snapshot, &entry));
                }

                CloseHandle(snapshot);

                return result;
            }

            DWORD GetProcessIdByName(const std::wstring& processName) const
            {
                HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

                if (snapshot == INVALID_HANDLE_VALUE)
                    return 0;

                DWORD pid = 0;
                PROCESSENTRY32W entry;
                entry.dwSize = sizeof(entry);

                if (Process32FirstW(snapshot, &entry))
                {
                    do
                    {
                        if (_wcsicmp(entry.szExeFile, processName.c_str()) == 0)
                        {
                            pid = entry.th32ProcessID;

                            break;
                        }
                    }
                    while (Process32NextW(snapshot, &entry));
                }

                CloseHandle(snapshot);

                return pid;
            }
    };
}

#endif
