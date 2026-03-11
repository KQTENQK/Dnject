#ifndef __UNII_MODELREGISTRY__
#define __UNII_MODELREGISTRY__

#include "IModel.h"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <string>

namespace Unii
{
    class ModelRegistry
    {
        private:
            std::unordered_map<std::type_index, std::unique_ptr<IModel>> m_uniqueModels;
            std::unordered_map<std::type_index, std::shared_ptr<IModel>> m_sharedModels;
            
        public:
            template<typename TModel>
            bool Register(std::unique_ptr<TModel> model)
            {
                static_assert(std::is_base_of_v<IModel, TModel>, 
                            "TModel must derive from IModel");
                
                auto typeIndex = std::type_index(typeid(TModel));
                
                if (m_uniqueModels.find(typeIndex) != m_uniqueModels.end() ||
                    m_sharedModels.find(typeIndex) != m_sharedModels.end())
                    return false;
                
                m_uniqueModels[typeIndex] = std::move(model);

                return true;
            }
            
            template<typename TModel>
            bool Register(std::shared_ptr<TModel> model)
            {
                static_assert(std::is_base_of_v<TModel, TModel>, 
                            "TModel must derive from TModel");
                
                auto typeIndex = std::type_index(typeid(TModel));
                
                if (m_uniqueModels.find(typeIndex) != m_uniqueModels.end()
                    || m_sharedModels.find(typeIndex) != m_sharedModels.end())
                {
                    return false;
                }
                
                m_sharedModels[typeIndex] = std::move(model);

                return true;
            }
            
            template<typename TModel>
            TModel* Get()
            {
                auto typeIndex = std::type_index(typeid(TModel));
                
                auto uniqueIt = m_uniqueModels.find(typeIndex);

                if (uniqueIt != m_uniqueModels.end())
                    return static_cast<TModel*>(uniqueIt->second.get());
                
                auto sharedIt = m_sharedModels.find(typeIndex);

                if (sharedIt != m_sharedModels.end())
                    return static_cast<TModel*>(sharedIt->second.get());
                
                return nullptr;
            }
            
            template<typename TModel>
            bool Unregister()
            {
                auto typeIndex = std::type_index(typeid(TModel));
                
                auto uniqueIt = m_uniqueModels.find(typeIndex);

                if (uniqueIt != m_uniqueModels.end())
                {
                    m_uniqueModels.erase(uniqueIt);

                    return true;
                }
                
                auto sharedIt = m_sharedModels.find(typeIndex);

                if (sharedIt != m_sharedModels.end())
                {
                    m_sharedModels.erase(sharedIt);

                    return true;
                }
                
                return false;
            }
            
            void Clear()
            {
                m_uniqueModels.clear();
                m_sharedModels.clear();
            }
            
            size_t GetModelCount() const
            {
                return m_uniqueModels.size() + m_sharedModels.size();
            }
    };
}

#endif
