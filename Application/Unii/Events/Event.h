#ifndef __UNII_EVENT__
#define __UNII_EVENT__

#include <mutex>
#include <vector>
#include <algorithm>
#include "IEvent.h"

namespace Unii
{
    template<typename... Args>
    class Event : public IEvent<Args...>
    {
        private:
            std::vector<Delegate<Args...>> m_delegates;
            mutable std::mutex m_mutex;
            
        public:
            void Subscribe(Delegate<Args...> delegate) override
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                
                auto it = std::find(m_delegates.begin(), m_delegates.end(), delegate);

                if (it == m_delegates.end())
                    m_delegates.push_back(std::move(delegate));
            }
            
            void Unsubscribe(Delegate<Args...> delegate) override
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                
                auto it = std::find(m_delegates.begin(), m_delegates.end(), delegate);
                
                if (it != m_delegates.end())
                    m_delegates.erase(it);
            }

            void UnsubscribeAll() override
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_delegates.clear();
            }
            
            void Invoke(Args... args)
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                for (const auto& delegate : m_delegates)
                    delegate.Invoke(args...);
            }

            size_t GetDelegateCount() const
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                return m_delegates.size();
            }

            IEvent<Args...>& operator+=(Delegate<Args...> delegate) override
            {
                Subscribe(std::move(delegate));

                return *this;
            }

            IEvent<Args...>& operator-=(Delegate<Args...> delegate) override
            {
                Unsubscribe(std::move(delegate));

                return *this;
            }
    };
}

#endif
