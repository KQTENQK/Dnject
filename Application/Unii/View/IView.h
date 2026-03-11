#ifndef __UNII_IVIEW__
#define __UNII_IVIEW__

#include <string>

namespace Unii
{
    class IView
    {
        private:
            std::string m_type;
            std::string m_category; 
            std::string m_displayName;
            bool m_visible;
            
        public:
            IView
            (
                const std::string& type,
                const std::string& category,
                const std::string& displayName
            )
            {
                m_type = type;
                m_category = category;
                m_displayName = displayName;
                m_visible = true;
            }

            const std::string& GetType() const
            {
                return m_type;
            }

            const std::string& GetCategory() const
            {
                return m_category;
            }

            const std::string& GetDisplayName() const
            {
                return m_displayName;
            }

            virtual ~IView() = default;
            virtual void Draw() = 0;

            virtual bool IsVisible()
            {
                return m_visible;
            }

            virtual bool* IsVisibleRaw()
            {
                return &m_visible;
            }

            virtual void SetVisible(bool visible)
            {
                m_visible = visible;
            }
    };
}

#endif
