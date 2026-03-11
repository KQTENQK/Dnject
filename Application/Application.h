#ifndef __APP_APPLICATION__
#define __APP_APPLICATION__

#include <memory>
#include "Unii/Unii.h"
#include "Model/MainModel.h"
#include "Model/ProcessBrowserModel.h"
#include "Presenter/MainPresenter.h"
#include "Presenter/ProcessBrowserPresenter.h"
#include "Service/ProcessQueryService.h"
#include "Service/InjectService.h"
#include "Service/FileDialogService.h"
#include "View/MainView.h"
#include "ApplicationContext.h"

namespace App
{
    class Application
    {
        public:
            Application() = default;

            void Initialize()
            {
                ApplicationContext& applicationContext = ApplicationContext::GetInstance();

                auto mainModel = std::make_shared<Dnject::MainModel>();
                auto processBrowserModel = std::make_shared<Dnject::ProcessBrowserModel>();

                auto fileDialogService = std::make_shared<Dnject::FileDialogService>();
                auto injectService = std::make_shared<Dnject::InjectService>();
                auto processQueryService = std::make_shared<Dnject::ProcessQueryService>();

                auto mainView = std::make_shared<Dnject::MainView>
                (
                    "Main",
                    "System",
                    "",
                    *mainModel,
                    *processBrowserModel
                );

                auto processBrowserPresenter = std::make_shared<Dnject::ProcessBrowserPresenter>
                (
                    *processBrowserModel,
                    *processQueryService,
                    *mainView
                );

                auto mainPresenter = std::make_shared<Dnject::MainPresenter>
                (
                    *mainModel,
                    *fileDialogService,
                    *processQueryService,
                    *injectService,
                    *processBrowserPresenter,
                    *mainView
                );

                applicationContext.Models().Register(mainModel);
                applicationContext.Models().Register(processBrowserModel);

                applicationContext.Services().Register(fileDialogService);
                applicationContext.Services().Register(injectService);
                applicationContext.Services().Register(processQueryService);

                applicationContext.Views().Register(mainView);

                applicationContext.Presenters().Register(mainPresenter);
                applicationContext.Presenters().Register(processBrowserPresenter);

                for (const auto& presenter : ApplicationContext::GetInstance().Presenters().GetAllPresenters())
                    presenter->Initialize();
            }

            void Shutdown()
            {
                ApplicationContext& applicationContext = ApplicationContext::GetInstance();

                applicationContext.Presenters().Clear();
                applicationContext.Views().Clear();
                applicationContext.Services().Clear();
                applicationContext.Models().Clear();
            }

            void Show()
            {
                for (const auto& view : ApplicationContext::GetInstance().Views().GetViewsInOrder())
                    if (view->IsVisible())
                        view->Draw();

                for (const auto& presenter : ApplicationContext::GetInstance().Presenters().GetAllPresenters())
                    presenter->Update();
            }

            static Application& GetInstance()
            {
                static Application instance;

                return instance;
            }
    };
}

#endif
