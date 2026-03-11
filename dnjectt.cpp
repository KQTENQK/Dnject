#include <iostream>
#include <string>
#include "Core/Injector.h"


int wmain(int argc, wchar_t* argv[])
{
    if (argc != 3)
    {
        std::wcerr << L"Usage: DnjectT.exe <processName> <dllPath>\n";

        return 1;
    }

    const wchar_t* processName = argv[1];
    const wchar_t* dllPath = argv[2];

    Dnject::Injector injector = Dnject::Injector();

    std::variant<bool, std::wstring> result = injector.Inject(processName, dllPath);

    if (std::holds_alternative<bool>(result))
    {
        if (std::get<bool>(result) == true)
        {
            std::wcout << L"Injection passed normally.";

            return 0;
        }
        
        std::wcerr << L"Injection failed.";

        return 2;
    }

    std::wcerr << std::get<std::wstring>(result);

    return 1;
}