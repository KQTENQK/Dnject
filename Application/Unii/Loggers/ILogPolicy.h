#ifndef __UNII_ILOGPOLICY__
#define __UNII_ILOGPOLICY__

#include "LogTypes.h"

namespace Unii
{
    class ILogPolicy
    {
        public:
            virtual ~ILogPolicy() = default;
            virtual void Write(LogLevel level, const std::string& message) = 0;
            virtual void Flush() = 0;
            virtual const char* GetName() const = 0;
    };
}

#endif
