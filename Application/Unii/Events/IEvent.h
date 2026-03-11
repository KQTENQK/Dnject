#ifndef __UNII_IEVENT__
#define __UNII_IEVENT__

#include "Delegate.h"

namespace Unii
{
    template<typename... Args>
    class IEvent
    {
    public:
        virtual ~IEvent() = default;
        
        virtual void Subscribe(Delegate<Args...> delegate) = 0;
        virtual void Unsubscribe(Delegate<Args...> delegate) = 0;
        virtual void UnsubscribeAll() = 0;
        virtual IEvent<Args...>& operator+=(Delegate<Args...> delegate) = 0;
        virtual IEvent<Args...>& operator-=(Delegate<Args...> delegate) = 0;
    };
}

#endif
