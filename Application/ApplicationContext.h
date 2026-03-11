#ifndef __APP_APPLICATIONCONTEXT__
#define __APP_APPLICATIONCONTEXT__

#include "Unii/Unii.h"
#include <memory>

namespace App
{
    class ApplicationContext
    {
        private:
            std::unique_ptr<Unii::ServiceRegistry> m_services;
            std::unique_ptr<Unii::ViewRegistry> m_views;
            std::unique_ptr<Unii::PresenterRegistry> m_presenters;
            std::unique_ptr<Unii::ModelRegistry> m_models;

        public:
            ApplicationContext(const ApplicationContext&) = delete;
            ApplicationContext& operator=(const ApplicationContext&) = delete;

            ApplicationContext()
            {
                m_services = std::make_unique<Unii::ServiceRegistry>();
                m_views = std::make_unique<Unii::ViewRegistry>();
                m_presenters = std::make_unique<Unii::PresenterRegistry>();
                m_models = std::make_unique<Unii::ModelRegistry>();
            }

            static ApplicationContext& GetInstance()
            {
                static ApplicationContext instance;

                return instance;
            }

            Unii::ServiceRegistry& Services()
            {
                return *m_services;
            }

            Unii::ViewRegistry& Views()
            {
                return *m_views;
            }

            Unii::PresenterRegistry& Presenters()
            {
                return *m_presenters;
            }

            Unii::ModelRegistry& Models()
            {
                return *m_models;
            }

            void ClearAll()
            {
                m_presenters->Clear();
                m_views->Clear();
                m_services->Clear();
                m_models->Clear();
            }
    };
}

#endif
