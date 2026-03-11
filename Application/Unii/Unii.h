#ifndef __UNII_ALLHEADERS__
#define __UNII_ALLHEADERS__

#include "Events/Events.h"
#include "Events/Delegate.h"
#include "Loggers/ConsoleLogPolicy.h"
#include "Loggers/FileLogPolicy.h"
#include "Loggers/ILogPolicy.h"
#include "Loggers/Logger.h"
#include "Loggers/LogTypes.h"
#include "Model/IModel.h"
#include "Model/ModelRegistry.h"
#include "Presenter/IPresenter.h"
#include "Presenter/PresenterRegistry.h"
#include "Service/IService.h"
#include "Service/ServiceRegistry.h"
#include "View/IView.h"
#include "View/ViewRegistry.h"
#include "View/Views/ConsoleView.h"
#include "ObservableData.h"
#include "ImGuiHelpers.h"
#include "WinApi/File/WinApiFileOperations.h"
#include "WinApi/GlobalInput/HotkeyRegistration.h"
#include "WinApi/GlobalInput/WinApiGlobalHotkeyManager.h"
#include "WinApi/GlobalInput/WinApiGlobalMouseWheelManager.h"

#endif
