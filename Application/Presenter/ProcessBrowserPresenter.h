#ifndef __PROCESSBROWSERPRESENTER__
#define __PROCESSBROWSERPRESENTER__

#include <filesystem>
#include "Unii/Unii.h"
#include "Model/ProcessBrowserModel.h"
#include "Service/ProcessQueryService.h"
#include "View/MainView.h"

namespace Dnject
{
    class ProcessBrowserPresenter : public Unii::IPresenter
    {
        private:
            ProcessBrowserModel& m_processBrowserModel;
            ProcessQueryService& m_processQueryService;

            MainView& m_mainView;

            Unii::Event<> m_processListUpdated;

        public:
            ProcessBrowserPresenter
            (
                ProcessBrowserModel& processBrowserModel,
                ProcessQueryService& processQueryService,
                MainView& mainView
            ) : m_processBrowserModel(processBrowserModel),
                m_processQueryService(processQueryService),
                m_mainView(mainView)
            {  }

            Unii::IEvent<>& ProcessListUpdated() { return m_processListUpdated; }

            void Initialize()
            {
                m_mainView.ProcessRefreshRequested() += Unii::MakeDelegate(this, &ProcessBrowserPresenter::RefreshProcesses);

                RefreshProcesses();
            }

            void Update() override {  }
            
            void Shutdown() override
            {
                m_mainView.ProcessRefreshRequested() -= Unii::MakeDelegate(this, &ProcessBrowserPresenter::RefreshProcesses);
            }

            void RefreshProcesses()
            {
                m_processBrowserModel.Processes() = m_processQueryService.GetProcesses();
                m_processListUpdated.Invoke();
            }
    };
}

#endif
