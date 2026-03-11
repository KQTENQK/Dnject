#ifndef __UNII_PRESENTERREGISTRY__
#define __UNII_PRESENTERREGISTRY__

#include "IPresenter.h"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <string>

namespace Unii
{
    class PresenterRegistry
    {
        private:
            std::unordered_map<std::type_index, std::unique_ptr<IPresenter>> m_uniquePresenters;
            std::unordered_map<std::type_index, std::shared_ptr<IPresenter>> m_sharedPresenters;
            std::vector<std::type_index> m_allPresenters;
            
        public:
            template<typename TPresenter>
            bool Register(std::unique_ptr<TPresenter> presenter)
            {
                static_assert(std::is_base_of_v<IPresenter, TPresenter>, 
                            "TPresenter must derive from IPresenter");
                
                auto typeIndex = std::type_index(typeid(TPresenter));
                
                if (m_uniquePresenters.find(typeIndex) != m_uniquePresenters.end()
                    || m_sharedPresenters.find(typeIndex) != m_sharedPresenters.end())
                {
                    return false;
                }
                
                m_uniquePresenters[typeIndex] = std::move(presenter);
                m_allPresenters.push_back(typeIndex);

                return true;
            }
            
            template<typename TPresenter>
            bool Register(std::shared_ptr<TPresenter> presenter)
            {
                static_assert(std::is_base_of_v<IPresenter, TPresenter>, 
                            "TPresenter must derive from IPresenter");
                
                auto typeIndex = std::type_index(typeid(TPresenter));
                
                if (m_uniquePresenters.find(typeIndex) != m_uniquePresenters.end()
                    || m_sharedPresenters.find(typeIndex) != m_sharedPresenters.end())
                {
                    return false;
                }
                
                m_sharedPresenters[typeIndex] = std::move(presenter);
                m_allPresenters.push_back(typeIndex);

                return true;
            }
            
            template<typename TPresenter>
            TPresenter* Get()
            {
                auto typeIndex = std::type_index(typeid(TPresenter));
                
                auto uniqueIt = m_uniquePresenters.find(typeIndex);

                if (uniqueIt != m_uniquePresenters.end())
                    return static_cast<TPresenter*>(uniqueIt->second.get());
                
                auto sharedIt = m_sharedPresenters.find(typeIndex);

                if (sharedIt != m_sharedPresenters.end())
                    return static_cast<TPresenter*>(sharedIt->second.get());
                
                return nullptr;
            }
            
            template<typename TPresenter>
            bool Unregister()
            {
                auto typeIndex = std::type_index(typeid(TPresenter));

                auto uniqueIt = m_uniquePresenters.find(typeIndex);

                if (uniqueIt != m_uniquePresenters.end())
                {
                    uniqueIt->second->Shutdown();
                    m_uniquePresenters.erase(uniqueIt);

                    auto it = std::find(m_allPresenters.begin(), m_allPresenters.end(), typeIndex);
                    
                    if (it != m_allPresenters.end())
                        m_allPresenters.erase(it);

                    return true;
                }
                
                auto sharedIt = m_sharedPresenters.find(typeIndex);

                if (sharedIt != m_sharedPresenters.end())
                {
                    auto it = std::find(m_allPresenters.begin(), m_allPresenters.end(), typeIndex);
                    
                    if (it != m_allPresenters.end())
                        m_allPresenters.erase(it);

                    sharedIt->second->Shutdown();
                    m_sharedPresenters.erase(sharedIt);

                    return true;
                }
                
                return false;
            }
            
            std::vector<IPresenter*> GetAllPresenters() const
            {
                std::vector<IPresenter*> result;

                for (const auto& typeIndex : m_allPresenters)
                {
                    auto uniqueIt = m_uniquePresenters.find(typeIndex);

                    if (uniqueIt != m_uniquePresenters.end())
                        result.push_back(uniqueIt->second.get());

                    auto sharedIt = m_sharedPresenters.find(typeIndex);

                    if (sharedIt != m_sharedPresenters.end())
                        result.push_back(sharedIt->second.get());
                }

                return result;
            }

            void Clear()
            {
                for (auto& [typeIndex, presenter] : m_uniquePresenters)
                    presenter->Shutdown();
                
                for (auto& [typeIndex, presenter] : m_sharedPresenters)
                    presenter->Shutdown();
                
                m_uniquePresenters.clear();
                m_sharedPresenters.clear();
            }
            
            size_t GetPresenterCount() const
            {
                return m_uniquePresenters.size() + m_sharedPresenters.size();
            }
    };
}

#endif
