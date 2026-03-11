#ifndef __FILEDIALOGSERVICE__
#define __FILEDIALOGSERVICE__

#include <string>
#include <windows.h>
#include <commdlg.h>
#include "Unii/Unii.h"

namespace Dnject
{
    class FileDialogService : public Unii::IService
    {
        public:
            std::wstring SelectFile(const wchar_t* filter) const
            {
                wchar_t fileBuffer[MAX_PATH] = { 0 };

                OPENFILENAMEW ofn = {};
                ofn.lStructSize = sizeof(ofn);
                ofn.lpstrFile = fileBuffer;
                ofn.nMaxFile = MAX_PATH;
                ofn.lpstrFilter = filter;
                ofn.nFilterIndex = 1;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileNameW(&ofn))
                    return fileBuffer;

                return L"";
            }
    };
}

#endif
