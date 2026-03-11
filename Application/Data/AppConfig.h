#ifndef __APPCONFIG__
#define __APPCONFIG__

#include <string>

namespace Dnject
{
    struct AppConfig
    {
        static inline const std::wstring SelectedProcessNameKey = L"SelectedProcessName";
        static inline const std::wstring SelectedFilePathKey = L"SelectedFilePath";

        std::wstring SelectedProcessName;
        std::wstring SelectedFilePath;

        AppConfig() = default;

        AppConfig
        (
            std::wstring selectedProcessName,
            std::wstring selectedFilePath
        )
        {
            SelectedProcessName = selectedProcessName;
            SelectedFilePath = selectedFilePath;
        }
    };
}

#endif
