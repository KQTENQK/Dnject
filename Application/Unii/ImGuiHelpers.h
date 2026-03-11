#ifndef __UNII_IMGUIHELPERS__
#define __UNII_IMGUIHELPERS__

#include <imgui.h>
#include "ObservableData.h"

namespace Unii
{
    bool InputTextDataBinding(const char* label, ObservableData<std::string>& binding)
    {
        char buffer[256];
        strcpy(buffer, binding.GetValue().c_str());
        
        if (ImGui::InputText(label, buffer, sizeof(buffer)))
        {
            binding = buffer;

            return true;
        }

        return false;
    }

    bool InputIntDataBinding(const char* label, ObservableData<int>& binding)
    {
        int value = binding;

        if (ImGui::InputInt(label, &value))
        {
            binding = value;

            return true;
        }

        return false;
    }

    bool ObservableCheckbox(const char* label, ObservableData<bool>& data)
    {
        bool temp = data;

        if (ImGui::Checkbox(label, &temp))
        {
            data = temp;

            return true;
        }
        
        return false;
    }
}

#endif
