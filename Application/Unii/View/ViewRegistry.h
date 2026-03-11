#ifndef __UNII_VIEWREGISTRY__
#define __UNII_VIEWREGISTRY__

#include <unordered_map>
#include <vector>
#include <string>
#include <typeindex>
#include <memory>
#include <algorithm>
#include "IView.h"

namespace Unii
{
    class ViewRegistry
    {
        private:
            std::unordered_map<std::type_index, std::unique_ptr<IView>> m_uniqueViews;
            std::unordered_map<std::type_index, std::shared_ptr<IView>> m_sharedViews;
            std::vector<std::type_index> m_viewOrder;

        public:
            template<typename TView>
            bool Register(std::unique_ptr<TView> view)
            {
                static_assert(std::is_base_of_v<IView, TView>, 
                            "TView must derive from IView");
                
                auto typeIndex = std::type_index(typeid(TView));
                
                if (m_uniqueViews.find(typeIndex) != m_uniqueViews.end())
                    return false;
                
                m_uniqueViews[typeIndex] = std::move(view);
                m_viewOrder.push_back(typeIndex);

                return true;
            }

            template<typename TView>
            bool Register(std::shared_ptr<TView> view)
            {
                static_assert(std::is_base_of_v<IView, TView>, 
                            "TView must derive from IView");
                
                auto typeIndex = std::type_index(typeid(TView));
                
                if (m_sharedViews.find(typeIndex) != m_sharedViews.end())
                    return false;
                
                m_sharedViews[typeIndex] = std::move(view);
                m_viewOrder.push_back(typeIndex);

                return true;
            }

            template<typename TView>
            bool Unregister()
            {
                auto typeIndex = std::type_index(typeid(TView));
                auto uniqueIt = m_uniqueViews.find(typeIndex);

                if (uniqueIt != m_uniqueViews.end())
                {
                    auto orderIt = std::find(m_viewOrder.begin(), m_viewOrder.end(), typeIndex);
                    
                    if (orderIt != m_viewOrder.end())
                        m_viewOrder.erase(orderIt);

                    m_uniqueViews.erase(uniqueIt);

                    return true;
                }

                auto sharedIt = m_sharedViews.find(typeIndex);

                if (sharedIt != m_sharedViews.end())
                {
                    auto orderIt = std::find(m_viewOrder.begin(), m_viewOrder.end(), typeIndex);
                    
                    if (orderIt != m_viewOrder.end())
                        m_viewOrder.erase(orderIt);

                    m_sharedViews.erase(sharedIt);

                    return true;
                }

                return false;
            }

            template<typename TView>
            TView* Get()
            {
                auto typeIndex = std::type_index(typeid(TView));

                auto uniqueIt = m_uniqueViews.find(typeIndex);

                if (uniqueIt != m_uniqueViews.end())
                    return static_cast<TView*>(uniqueIt->second.get());

                auto sharedIt = m_sharedViews.find(typeIndex);

                if (sharedIt != m_sharedViews.end())
                    return static_cast<TView*>(sharedIt->second.get());
                
                return nullptr;
            }

            IView* Get(std::type_index typeIndex) const
            {
                auto uniqueIt = m_uniqueViews.find(typeIndex);
                
                if (uniqueIt != m_uniqueViews.end())
                    return uniqueIt->second.get();

                auto sharedIt = m_sharedViews.find(typeIndex);
                
                if (sharedIt != m_sharedViews.end())
                    return sharedIt->second.get();
                
                return nullptr;
            }

            std::vector<IView*> GetViewsInOrder() const
            {
                std::vector<IView*> result;

                for (const auto& typeIndex : m_viewOrder)
                {
                    auto uniqueIt = m_uniqueViews.find(typeIndex);

                    if (uniqueIt != m_uniqueViews.end())
                        result.push_back(uniqueIt->second.get());

                    auto sharedIt = m_sharedViews.find(typeIndex);

                    if (sharedIt != m_sharedViews.end())
                        result.push_back(sharedIt->second.get());
                }

                return result;
            }

            std::vector<IView*> GetViewsByCategory(const std::string& category) const
            {
                std::vector<IView*> result;

                for (const auto& [typeIndex, view] : m_uniqueViews)
                    if (view->GetCategory() == category)
                        result.push_back(view.get());

                for (const auto& [typeIndex, view] : m_sharedViews)
                    if (view->GetCategory() == category)
                        result.push_back(view.get());
                
                return result;
            }

            std::vector<std::string> GetViewCategories() const
            {
                std::vector<std::string> categories;

                for (const auto& [typeIndex, view] : m_uniqueViews)
                    categories.push_back(view->GetCategory());

                for (const auto& [typeIndex, view] : m_sharedViews)
                    categories.push_back(view->GetCategory());
                
                std::sort(categories.begin(), categories.end());
                categories.erase(std::unique(categories.begin(), categories.end()), categories.end());
                
                return categories;
            }

            void Clear()
            {
                m_uniqueViews.clear();
                m_sharedViews.clear();
                m_viewOrder.clear();
            }

            size_t GetViewCount() const
            {
                return m_uniqueViews.size() + m_sharedViews.size();
            }
    };
}

#endif
