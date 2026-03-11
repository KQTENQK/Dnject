#ifndef __UNII_LOGGER__
#define __UNII_LOGGER__

#include "ILogPolicy.h"
#include "LogTypes.h"
#include "Unii/Events/Events.h"
#include <vector>
#include <memory>
#include <mutex>

namespace Unii
{
    class Logger
    {
        private:
            std::vector<std::unique_ptr<ILogPolicy>> m_policies;
            std::mutex m_mutex;
            LogLevel m_minLevel;
            Event<> m_writeRequested;

            Logger()
            {
                m_minLevel = LogLevel::Info;
            }

            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;

        public:
            IEvent<>& WriteRequested()
            {
                return m_writeRequested;
            }

            static Logger& GetInstance()
            {
                static Logger instance;

                return instance;
            }

            template<typename T, typename... Args>
            T* AddPolicy(Args&&... args)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                auto policy = std::make_unique<T>(std::forward<Args>(args)...);
                T* ptr = policy.get();
                m_policies.push_back(std::move(policy));

                return ptr;
            }

            Logger& AddPolicy(std::unique_ptr<ILogPolicy> policy)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_policies.push_back(std::move(policy));

                return *this;
            }

            template<typename T>
            T* GetPolicy()
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                for (auto& policy : m_policies)
                    if (auto* casted = dynamic_cast<T*>(policy.get()))
                        return casted;

                return nullptr;
            }

            Logger& SetMinLevel(LogLevel level)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_minLevel = level;

                return *this;
            }

            LogLevel GetMinLevel() const
            {
                return m_minLevel;
            }

            void Log(LogLevel level, const std::string& message)
            {
                if (static_cast<int>(level) < static_cast<int>(m_minLevel))
                    return;

                std::lock_guard<std::mutex> lock(m_mutex);

                for (auto& policy : m_policies)
                    policy->Write(level, message);

                m_writeRequested.Invoke();
            }

            void Trace(const std::string& message)
            {
                Log(LogLevel::Trace, message);
            }

            void Debug(const std::string& message)
            {
                Log(LogLevel::Debug, message);
            }

            void Info(const std::string& message)
            {
                Log(LogLevel::Info, message);
            }

            void Warning(const std::string& message)
            {
                Log(LogLevel::Warning, message);
            }

            void Error(const std::string& message)
            {
                Log(LogLevel::Error, message);
            }

            void Critical(const std::string& message)
            {
                Log(LogLevel::Critical, message);
            }

            void User(const std::string& message)
            {
                Log(LogLevel::User, message);
            }

            template<typename... Args>
            void TraceF(const char* format, Args&&... args)
            {
                char buffer[1024];
                snprintf(buffer, sizeof(buffer), format, std::forward<Args>(args)...);
                Trace(buffer);
            }

            template<typename... Args>
            void DebugF(const char* format, Args&&... args)
            {
                char buffer[1024];
                snprintf(buffer, sizeof(buffer), format, std::forward<Args>(args)...);
                Debug(buffer);
            }

            template<typename... Args>
            void InfoF(const char* format, Args&&... args)
            {
                char buffer[1024];
                snprintf(buffer, sizeof(buffer), format, std::forward<Args>(args)...);
                Info(buffer);
            }

            template<typename... Args>
            void WarningF(const char* format, Args&&... args)
            {
                char buffer[1024];
                snprintf(buffer, sizeof(buffer), format, std::forward<Args>(args)...);
                Warning(buffer);
            }

            template<typename... Args>
            void ErrorF(const char* format, Args&&... args)
            {
                char buffer[1024];
                snprintf(buffer, sizeof(buffer), format, std::forward<Args>(args)...);
                Error(buffer);
            }

            template<typename... Args>
            void CriticalF(const char* format, Args&&... args)
            {
                char buffer[1024];
                snprintf(buffer, sizeof(buffer), format, std::forward<Args>(args)...);
                Critical(buffer);
            }

            template<typename... Args>
            void UserF(const char* format, Args&&... args)
            {
                char buffer[1024];
                snprintf(buffer, sizeof(buffer), format, std::forward<Args>(args)...);
                User(buffer);
            }

            void FlushAll()
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                for (auto& policy : m_policies)
                    policy->Flush();
            }

            void ClearPolicies()
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_policies.clear();
            }

            size_t GetPolicyCount()
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                return m_policies.size();
            }
    };
}

#endif
