#ifndef __PROCESSITEM__
#define __PROCESSITEM__

#include <string>
#include <windows.h>

namespace Dnject
{
    struct ProcessItem
    {
        DWORD Id;
        std::wstring Name;

        ProcessItem()
        {
            Id = 0;
        }

        ProcessItem
        (
            DWORD id,
            std::wstring name
        )
        {
            Id = id;
            Name = name;
        }
    };
}

#endif
