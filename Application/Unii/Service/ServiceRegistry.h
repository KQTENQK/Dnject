#ifndef __UNII_SERVICEREGISTRY__
#define __UNII_SERVICEREGISTRY__

#include "IService.h"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <string>

namespace Unii
{
    class ServiceRegistry
    {
        private:
            std::unordered_map<std::type_index, std::unique_ptr<IService>> m_uniqueServices;
            std::unordered_map<std::type_index, std::shared_ptr<IService>> m_sharedServices;
            
        public:
            template<typename TService>
            bool Register(std::unique_ptr<TService> service)
            {
                static_assert(std::is_base_of_v<IService, TService>, 
                            "TService must derive from IService");
                
                auto typeIndex = std::type_index(typeid(TService));
                
                if (m_uniqueServices.find(typeIndex) != m_uniqueServices.end() ||
                    m_sharedServices.find(typeIndex) != m_sharedServices.end())
                    return false;
                
                m_uniqueServices[typeIndex] = std::move(service);
                return true;
            }
            
            template<typename TService>
            bool Register(std::shared_ptr<TService> service)
            {
                static_assert(std::is_base_of_v<IService, TService>, 
                            "TService must derive from IService");
                
                auto typeIndex = std::type_index(typeid(TService));
                
                if (m_uniqueServices.find(typeIndex) != m_uniqueServices.end()
                    || m_sharedServices.find(typeIndex) != m_sharedServices.end())
                {
                    return false;
                }
                
                m_sharedServices[typeIndex] = std::move(service);

                return true;
            }
            
            template<typename TService>
            TService* Get()
            {
                auto typeIndex = std::type_index(typeid(TService));
                
                auto uniqueIt = m_uniqueServices.find(typeIndex);

                if (uniqueIt != m_uniqueServices.end())
                    return static_cast<TService*>(uniqueIt->second.get());
                
                auto sharedIt = m_sharedServices.find(typeIndex);

                if (sharedIt != m_sharedServices.end())
                    return static_cast<TService*>(sharedIt->second.get());
                
                return nullptr;
            }
            
            template<typename TService>
            bool Unregister()
            {
                auto typeIndex = std::type_index(typeid(TService));
                
                auto uniqueIt = m_uniqueServices.find(typeIndex);

                if (uniqueIt != m_uniqueServices.end())
                {
                    m_uniqueServices.erase(uniqueIt);

                    return true;
                }
                
                auto sharedIt = m_sharedServices.find(typeIndex);

                if (sharedIt != m_sharedServices.end())
                {
                    m_sharedServices.erase(sharedIt);

                    return true;
                }
                
                return false;
            }
            
            void Clear()
            {
                m_uniqueServices.clear();
                m_sharedServices.clear();
            }
            
            size_t GetServiceCount() const
            {
                return m_uniqueServices.size() + m_sharedServices.size();
            }
    };
}

#endif
