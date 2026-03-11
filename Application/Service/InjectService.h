#ifndef __INJECTSERVICE__
#define __INJECTSERVICE__

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include "Unii/Unii.h"
#include "Core/Injector.h"

namespace Dnject
{
    class InjectService : public Unii::IService
    {
        private:
            Injector m_injector;

        public:
            InjectService()
            {
                m_injector = Injector();
            }

            std::wstring Inject
            (
                DWORD processId,
                const std::wstring dllPath
            )
            {
                std::variant<bool, std::wstring> result = m_injector.Inject(processId, dllPath);

                if (std::holds_alternative<bool>(result))
                {
                    if (std::get<bool>(result) == true)
                        return L"Injection passed normally.";
                    
                    return L"Injection failed.";
                }

                return std::get<std::wstring>(result);
            }

            std::wstring Inject
            (
                const std::wstring processName,
                const std::wstring dllPath
            )
            {
                std::variant<bool, std::wstring> result = m_injector.Inject(processName, dllPath);

                if (std::holds_alternative<bool>(result))
                {
                    if (std::get<bool>(result) == true)
                        return L"Injection passed normally.";
                    
                    return L"Injection failed.";
                }

                return std::get<std::wstring>(result);
            }
    };
}

#endif
