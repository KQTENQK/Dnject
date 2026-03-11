#ifndef __UNII_LOGTYPES__
#define __UNII_LOGTYPES__

#include <string>
#include <imgui.h>

namespace Unii
{
    enum class LogLevel
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical,
        User
    };

    inline const char* LogLevelToString(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::Trace:    return "TRACE";
            case LogLevel::Debug:    return "DEBUG";
            case LogLevel::Info:     return "INFO";
            case LogLevel::Warning:  return "WARN";
            case LogLevel::Error:    return "ERROR";
            case LogLevel::Critical: return "CRITICAL";
            case LogLevel::User:     return "USER";
            default:                 return "UNKNOWN";
        }
    }

    inline ImVec4 LogLevelToColor(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::Trace:    return ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
            case LogLevel::Debug:    return ImVec4(0.5f, 0.5f, 1.0f, 1.0f);
            case LogLevel::Info:     return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            case LogLevel::Warning:  return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            case LogLevel::Error:    return ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
            case LogLevel::Critical: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            case LogLevel::User:     return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            default:                 return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }

    struct LogEntry
    {
        LogLevel Level;
        std::string Message;
        std::string Timestamp;
        uint64_t Id;

        LogEntry
        (
            LogLevel level,
            const std::string& message,
            const std::string& timestamp,
            uint64_t id
        )
        {
            Level = level;
            Message = message;
            Timestamp = timestamp;
            Id = id;
        }
    };
}

#endif
