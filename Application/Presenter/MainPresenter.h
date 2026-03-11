#ifndef __MAINPRESENTER__
#define __MAINPRESENTER__

#include "Unii/Unii.h"
#include "Model/MainModel.h"
#include "Service/FileDialogService.h"
#include "Service/ProcessQueryService.h"
#include "Service/InjectService.h"
#include "Presenter/ProcessBrowserPresenter.h"
#include "View/MainView.h"

namespace Dnject
{
    class MainPresenter : public Unii::IPresenter
    {
        private:
            MainModel& m_mainModel;
            FileDialogService& m_fileDialogService;
            ProcessQueryService& m_processQueryService;
            InjectService& m_injectService;
            ProcessBrowserPresenter& m_processBrowserPresenter;
            MainView& m_mainView;

        public:
            MainPresenter
            (
                MainModel& mainModel,
                FileDialogService& fileDialogService,
                ProcessQueryService& processQueryService,
                InjectService& injectService,
                ProcessBrowserPresenter& processBrowserPresenter,
                MainView& mainView
            ) : m_mainModel(mainModel),
                m_fileDialogService(fileDialogService),
                m_processQueryService(processQueryService),
                m_injectService(injectService),
                m_processBrowserPresenter(processBrowserPresenter),
                m_mainView(mainView)
            {  }

            void Initialize() override
            {
                m_processBrowserPresenter.ProcessListUpdated() += Unii::MakeDelegate(this, &MainPresenter::OnProcessListUpdated);
                m_mainView.FileBrowseRequested() += Unii::MakeDelegate(this, &MainPresenter::BrowseFile);
                m_mainView.InjectRequested() += Unii::MakeDelegate(this, &MainPresenter::OnInjectRequested);
            }

            void Update() override {  }
            
            void Shutdown() override
            {
                m_processBrowserPresenter.ProcessListUpdated() -= Unii::MakeDelegate(this, &MainPresenter::OnProcessListUpdated);
                m_mainView.FileBrowseRequested() -= Unii::MakeDelegate(this, &MainPresenter::BrowseFile);
                m_mainView.InjectRequested() -= Unii::MakeDelegate(this, &MainPresenter::OnInjectRequested);
            }

        private:
            void OnProcessListUpdated()
            {
                m_mainModel.SetStatusMessage(L"Process list updated.");
            }

            void OnInjectRequested()
            {
                std::wstring message = m_injectService.Inject(m_mainModel.Config().SelectedProcessName, m_mainModel.Config().SelectedFilePath);
                m_mainModel.SetStatusMessage(message);
            }

            void ApplyConfig(const std::wstring& processName, const std::wstring& filePath)
            {
                m_mainModel.Config().SelectedProcessName = processName;
                m_mainModel.Config().SelectedFilePath = filePath;
            }

            void BrowseFile()
            {
                std::wstring filePath = m_fileDialogService.SelectFile(L"Dynamic Link Library (*.dll)\0*.dll\0All Files\0*.*\0");

                if (filePath.empty())
                    return;

                m_mainModel.Config().SelectedFilePath = filePath;
                m_mainModel.SetStatusMessage(L"DLL selected.");
            }

            bool IsConfigValid()
            {
                if (m_mainModel.Config().SelectedProcessName.empty())
                {
                    m_mainModel.SetStatusMessage(L"Process is not selected.");

                    return false;
                }

                if (m_mainModel.Config().SelectedFilePath.empty())
                {
                    m_mainModel.SetStatusMessage(L"File path is empty.");

                    return false;
                }

                DWORD pid = m_processQueryService.GetProcessIdByName(m_mainModel.Config().SelectedProcessName);

                if (pid == 0)
                {
                    m_mainModel.SetStatusMessage(L"Selected process was not found.");

                    return false;
                }

                m_mainModel.SetStatusMessage(L"Ok.");

                return true;
            }
    };
}

#endif
