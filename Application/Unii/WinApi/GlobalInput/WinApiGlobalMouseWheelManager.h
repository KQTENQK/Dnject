#ifndef __UNII_WINAPIGLOBALMOUSEWHEELMANAGER__
#define __UNII_WINAPIGLOBALMOUSEWHEELMANAGER__

#include <windows.h>
#include <functional>
#include <atomic>
#include <mutex>
#include <vector>

namespace Unii
{
    class WinApiGlobalMouseWheelManager
    {
        private:
            static HHOOK m_mouseHook;
            static std::atomic<bool> m_hookInstalled;
            static std::mutex m_mutex;
            
            static std::function<void()> m_wheelUpCallback;
            static std::function<void()> m_wheelDownCallback;
            static std::atomic<bool> m_enabled;

        public:
            static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
            {
                if (nCode >= 0 && m_enabled)
                {
                    if (wParam == WM_MOUSEWHEEL)
                    {
                        MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;
                        int delta = GET_WHEEL_DELTA_WPARAM(pMouseStruct->mouseData);
                        
                        std::lock_guard<std::mutex> lock(m_mutex);
                        if (delta > 0 && m_wheelUpCallback)
                            m_wheelUpCallback();

                        else if (delta < 0 && m_wheelDownCallback)
                            m_wheelDownCallback();
                    }
                }

                return CallNextHookEx(m_mouseHook, nCode, wParam, lParam);
            }

            static void InstallHook()
            {
                if (!m_hookInstalled)
                {
                    m_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(nullptr), 0);
                    m_hookInstalled = (m_mouseHook != nullptr);
                    m_enabled = true;
                }
            }

            static void UninstallHook()
            {
                if (m_hookInstalled)
                {
                    UnhookWindowsHookEx(m_mouseHook);
                    m_mouseHook = nullptr;
                    m_hookInstalled = false;
                    m_enabled = false;
                }
            }

            static void SetWheelUpCallback(std::function<void()> callback)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_wheelUpCallback = callback;
            }

            static void SetWheelDownCallback(std::function<void()> callback)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_wheelDownCallback = callback;
            }

            static void SetEnabled(bool enabled)
            {
                m_enabled = enabled;
            }

            static void ClearCallbacks()
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_wheelUpCallback = nullptr;
                m_wheelDownCallback = nullptr;
            }
    };

    HHOOK WinApiGlobalMouseWheelManager::m_mouseHook = nullptr;
    std::atomic<bool> WinApiGlobalMouseWheelManager::m_hookInstalled = false;
    std::mutex WinApiGlobalMouseWheelManager::m_mutex;
    std::function<void()> WinApiGlobalMouseWheelManager::m_wheelUpCallback = nullptr;
    std::function<void()> WinApiGlobalMouseWheelManager::m_wheelDownCallback = nullptr;
    std::atomic<bool> WinApiGlobalMouseWheelManager::m_enabled = false;
}

#endif
