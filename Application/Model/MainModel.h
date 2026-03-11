#ifndef __MAINMODEL__
#define __MAINMODEL__

#include <string>
#include "Unii/Unii.h"
#include "Data/AppConfig.h"

namespace Dnject
{
    class MainModel : public Unii::IModel
    {
        private:
            AppConfig m_config;
            std::wstring m_statusMessage;

        public:
            MainModel()
            {
                m_config = AppConfig();
            }

            AppConfig& Config()
            {
                return m_config;
            }

            const AppConfig& Config() const
            {
                return m_config;
            }

            void SetStatusMessage(const std::wstring& message)
            {
                m_statusMessage = message;
            }

            const std::wstring& GetStatusMessage() const
            {
                return m_statusMessage;
            }
    };
}

#endif
