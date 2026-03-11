#ifndef __UNII_OBSERVABLEDATA__
#define __UNII_OBSERVABLEDATA__

#include "Events/Events.h"

namespace Unii
{
    template<typename T, typename Compare = std::not_equal_to<T>>
    class ObservableData
    {
        private:
            T m_value;
            Event<const T&> m_dataChanged;
            Compare m_comparator;
        
        public:
            ObservableData() : m_value(T()), m_comparator(Compare()) {  }

            ObservableData(const T& value, Compare comparator = Compare())
            {
                m_value = value;
                m_comparator = comparator;
            }

            IEvent<const T&>& DataChanged()
            {
                return m_dataChanged;
            }
            
            operator T() const
            {
                return m_value;
            }

            const T& GetValue() const
            {
                return m_value;
            }

            void ForceSetValue(const T& newValue)
            {
                m_value = newValue;
                m_dataChanged.Invoke(m_value);
            }
            
            ObservableData& operator=(const T& newValue)
            {
                if (m_comparator(m_value, newValue))
                {
                    m_value = newValue;

                    m_dataChanged.Invoke(m_value);
                }

                return *this;
            }
    };
}

#endif
