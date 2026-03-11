#ifndef __UNII_WINAPIGLOBALHOTKEYMANAGER__
#define __UNII_WINAPIGLOBALHOTKEYMANAGER__

#include "HotkeyRegistration.h"
#include "Loggers/Logger.h"
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

namespace Unii
{
    class WinApiGlobalHotkeyManager
    {
        private:
            std::unordered_map<int, HotkeyRegistration> m_hotkeyRegistry;
            std::vector<HotkeyRegistration> m_appliedHotkeys;
            std::atomic<bool> m_isRunning;
            std::atomic<bool> m_needThreadHotkeyUpdate;
            std::thread m_hotkeyThread;
            std::mutex m_mutex;
            static int m_nextId;

        public:
            WinApiGlobalHotkeyManager()
            {
                m_isRunning = false;
            }

            static WinApiGlobalHotkeyManager& GetInstance()
            {
                static WinApiGlobalHotkeyManager instance;
                
                return instance;
            }

            void UpdateHotkeys()
            {
                if (m_needThreadHotkeyUpdate)
                {
                    ReleaseHotkeys();

                    for (const auto& pair : m_hotkeyRegistry)
                    {
                        if (RegisterHotKey(NULL, pair.second.Id, pair.second.Modifiers, pair.second.Key))
                        {
                            m_appliedHotkeys.push_back
                            (
                                HotkeyRegistration
                                {
                                    pair.second.Id,
                                    pair.second.Modifiers,
                                    pair.second.Key,
                                    pair.second.Callback,
                                    pair.second.Enabled
                                }
                            );

                            Logger::GetInstance().DebugF
                            (
                                "Registered hotkey: ID=%d, Modifiers=0x%X, Key=0x%X", 
                                pair.second.Id, pair.second.Modifiers, pair.second.Key
                            );

                            continue;
                        }

                        Logger::GetInstance().ErrorF
                        (
                            "Failed to register hotkey: Modifiers=0x%X, Key=0x%X", 
                            pair.second.Modifiers, pair.second.Key
                        );
                    }

                    m_needThreadHotkeyUpdate = false;
                }
            }

            void ReleaseHotkeys()
            {
                for (auto& registraion : m_appliedHotkeys)
                {
                    if (UnregisterHotKey(NULL, registraion.Id))
                        Logger::GetInstance().DebugF("Unregistered hotkey: ID=%d", registraion.Id);
                    else
                        Logger::GetInstance().ErrorF("Failed to unregister hotkey: ID=%d", registraion.Id);
                }

                m_appliedHotkeys.clear();
            }

            void HotkeyThreadProc()
            {
                MSG msg;
                PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
                
                while (m_isRunning)
                {
                    UpdateHotkeys();

                    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                    {
                        if (msg.message == WM_HOTKEY)
                        {
                            std::lock_guard<std::mutex> lock(m_mutex);
                            int hotkeyId = static_cast<int>(msg.wParam);
                            auto it = m_hotkeyRegistry.find(hotkeyId);

                            if (it != m_hotkeyRegistry.end() && it->second.Enabled)
                            {
                                try
                                {
                                    it->second.Callback();
                                }
                                catch (const std::exception& e)
                                {
                                    Logger::GetInstance().ErrorF("Hotkey callback error: %s", e.what());
                                }
                            }
                        }

                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }

                std::lock_guard<std::mutex> lock(m_mutex);

                ReleaseHotkeys();
            }

            void RegisterHotkey(UINT modifiers, UINT key, std::function<void()> callback)
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                int id = m_nextId++;

                m_hotkeyRegistry[id] = HotkeyRegistration { id, modifiers, key, callback, true };

                m_needThreadHotkeyUpdate = true;
            }

            bool UnregisterHotkey(int id)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                
                auto it = m_hotkeyRegistry.find(id);

                if (it != m_hotkeyRegistry.end())
                {
                    m_hotkeyRegistry.erase(it);
                    Logger::GetInstance().DebugF("Unregistered hotkey: ID=%d", id);

                    return true;
                }

                Logger::GetInstance().ErrorF("Failed to unregister hotkey (registration not found): ID=%d", id);

                return false;
            }

            void UnregisterAll()
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                m_hotkeyRegistry.clear();

                m_nextId = 1;
            }

            void StartListening()
            {
                if (!m_isRunning)
                {
                    m_isRunning = true;
                    m_hotkeyThread = std::thread(&WinApiGlobalHotkeyManager::HotkeyThreadProc, this);

                    Logger::GetInstance().Debug("Hotkey listning started.");
                }
            }

            void StopListening()
            {
                m_isRunning = false;

                if (m_hotkeyThread.joinable())
                    m_hotkeyThread.join();
                
                UnregisterAll();    
                Logger::GetInstance().Debug("Hotkey listning stopped.");
            }

            size_t GetRegisteredCount()
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                return m_hotkeyRegistry.size();
            }

            ~WinApiGlobalHotkeyManager()
            {
                StopListening();
            }
    };

    int WinApiGlobalHotkeyManager::m_nextId = 1;
}

#endif
