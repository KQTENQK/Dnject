#ifndef __UNII_HOTKEYREGISTRATION__
#define __UNII_HOTKEYREGISTRATION__

#include <windows.h>
#include <functional>

namespace Unii
{
    struct HotkeyRegistration
    {
        int Id;
        UINT Modifiers;
        UINT Key;
        std::function<void()> Callback;
        bool Enabled;
    };
}

#endif
