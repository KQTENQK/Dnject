#ifndef __UNII_CONSOLELOGPOLICY__
#define __UNII_CONSOLELOGPOLICY__

#include "ILogPolicy.h"
#include "LogTypes.h"
#include <vector>
#include <mutex>
#include <chrono>

namespace Unii
{
    class ConsoleLogPolicy : public ILogPolicy
    {
        private:
            std::vector<LogEntry> m_logs;
            std::mutex m_mutex;
            size_t m_maxLogs;
            uint64_t m_nextId;

            std::string GetCurrentTimestamp()
            {
                auto now = std::chrono::system_clock::now();
                auto time_t = std::chrono::system_clock::to_time_t(now);
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now.time_since_epoch()) % 1000;
                
                char buffer[64];
                std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&time_t));

                return std::string(buffer) + "." + std::to_string(ms.count());
            }

        public:
            ConsoleLogPolicy(size_t maxLogs = 1000)
            {
                m_maxLogs = maxLogs;
                m_nextId = 0;
            }

            void Write(LogLevel level, const std::string& message) override
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_logs.emplace_back(level, message, GetCurrentTimestamp(), m_nextId++);
                
                if (m_logs.size() > m_maxLogs)
                    m_logs.erase(m_logs.begin());
            }

            void Flush() override {}

            const char* GetName() const override
            {
                return "ConsoleLogPolicy";
            }

            std::vector<LogEntry> GetLogs()
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                return m_logs;
            }

            void Clear()
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_logs.clear();
            }

            size_t GetLogCount()
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                return m_logs.size();
            }

            size_t GetMaxLogs() const
            {
                return m_maxLogs;
            }

            void SetMaxLogs(size_t maxLogs)
            {
                m_maxLogs = maxLogs;
            }
    };
}

#endif
