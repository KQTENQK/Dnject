#ifndef __UNII_CONSOLEVIEW__
#define __UNII_CONSOLEVIEW__

#include "View/IView.h"
#include "Loggers/Logger.h"
#include "Loggers/ConsoleLogPolicy.h"
#include "Loggers/LogTypes.h"
#include "ObservableData.h"
#include "ImGuiHelpers.h"
#include <vector>
#include <string>
#include <imgui.h>

namespace Unii
{
    class ConsoleView : public IView
    {
        private:
            ConsoleLogPolicy* m_consolePolicy;
            
            bool m_autoScroll;
            char m_inputBuffer[1024];
            
            ObservableData<bool> m_showDebug;
            ObservableData<bool> m_showTrace;
            ObservableData<bool> m_showInfo;
            ObservableData<bool> m_showWarning;
            ObservableData<bool> m_showError;
            ObservableData<bool> m_showCritical;
            ObservableData<bool> m_showUser;

            mutable std::vector<LogEntry> m_filteredLogs;
            mutable bool m_filterCacheDirty;

        public:
            ConsoleView
            (
                const std::string& type,
                const std::string& category,
                const std::string& displayName
            ) : IView(type, category, displayName)
            {
                m_autoScroll = true;
                m_showTrace = true;
                m_showDebug = true;
                m_showInfo = true;
                m_showWarning = true;
                m_showError = true;
                m_showCritical = true;
                m_showUser = true;
                m_filterCacheDirty = true;

                m_showTrace.DataChanged() += Unii::MakeDelegate(std::function([this](const bool&) { SetNeedCacheUpdate(); }));
                m_showDebug.DataChanged() += Unii::MakeDelegate(std::function([this](const bool&) { SetNeedCacheUpdate(); }));
                m_showInfo.DataChanged() += Unii::MakeDelegate(std::function([this](const bool&) { SetNeedCacheUpdate(); }));
                m_showWarning.DataChanged() += Unii::MakeDelegate(std::function([this](const bool&) { SetNeedCacheUpdate(); }));
                m_showError.DataChanged() += Unii::MakeDelegate(std::function([this](const bool&) { SetNeedCacheUpdate(); }));
                m_showCritical.DataChanged() += Unii::MakeDelegate(std::function([this](const bool&) { SetNeedCacheUpdate(); }));
                m_showUser.DataChanged() += Unii::MakeDelegate(std::function([this](const bool&) { SetNeedCacheUpdate(); }));

                m_inputBuffer[0] = '\0';
                
                m_consolePolicy = Logger::GetInstance().GetPolicy<ConsoleLogPolicy>();
                Logger::GetInstance().WriteRequested() += Unii::MakeDelegate(this, &ConsoleView::SetNeedCacheUpdate);

                int const MAX_LOGS = 1000;

                if (!m_consolePolicy)
                {
                    Logger::GetInstance().AddPolicy(std::move(std::make_unique<ConsoleLogPolicy>(MAX_LOGS)));
                    m_consolePolicy = Logger::GetInstance().GetPolicy<ConsoleLogPolicy>();
                }
            }

            ~ConsoleView()
            {
                Logger::GetInstance().WriteRequested() -= Unii::MakeDelegate(this, &ConsoleView::SetNeedCacheUpdate);
            }
            
            void Draw() override
            {
                ImGui::Begin(GetDisplayName().c_str(), IsVisibleRaw());

                DrawControlPanel();
                ImGui::Separator();
                DrawLogContents();
                ImGui::Separator();
                DrawInputPanel();

                ImGui::End();
            }

        private:
            void DrawControlPanel()
            {
                if (ImGui::Button("Clear"))
                {
                    if (m_consolePolicy)
                        m_consolePolicy->Clear();
                    
                    m_filterCacheDirty = true;
                }
                    
                ImGui::SameLine();
                ImGui::Checkbox("Auto-scroll", &m_autoScroll);
                
                ImGui::SameLine();

                if (ImGui::Button("Copy All"))
                    CopyAllToClipboard();

                ImGui::SameLine();
                ImGui::Text("Filters:");
                ImGui::SameLine();
                ObservableCheckbox("Trace", m_showTrace);
                ImGui::SameLine();
                ObservableCheckbox("Debug", m_showDebug); 
                ImGui::SameLine();
                ObservableCheckbox("Info", m_showInfo); 
                ImGui::SameLine();
                ObservableCheckbox("Warn", m_showWarning);
                ImGui::SameLine();
                ObservableCheckbox("Error", m_showError);
                ImGui::SameLine();
                ObservableCheckbox("Crit", m_showCritical);
                ImGui::SameLine();
                ObservableCheckbox("User", m_showUser);
            }

            void DrawLogContents()
            {
                ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), 
                                false, ImGuiWindowFlags_HorizontalScrollbar);

                const auto& logs = GetFilteredLogs();
                
                for (const auto& log : logs)
                    DrawLogEntry(log);
                
                if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
                    
                ImGui::EndChild();
            }

            void DrawInputPanel()
            {
                ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue;

                if (ImGui::InputText("##Input", m_inputBuffer, sizeof(m_inputBuffer), inputFlags))
                    SendUserMessage();
                
                ImGui::SameLine();
                
                if (ImGui::Button("Send") 
                    || (ImGui::IsKeyPressed(ImGuiKey_Enter) && ImGui::IsWindowFocused()))
                {
                    SendUserMessage();
                }
                
                ImGui::SameLine();
                ImGui::TextDisabled("(Press Enter to send)");
            }

            void DrawLogEntry(const LogEntry& entry)
            {
                ImVec4 color = LogLevelToColor(entry.Level);
                ImGui::PushStyleColor(ImGuiCol_Text, color);

                ImGui::Text
                (
                    "[%s] [%s] %s", 
                    entry.Timestamp.c_str(), 
                    LogLevelToString(entry.Level), 
                    entry.Message.c_str()
                );

                ImGui::PopStyleColor();
            }

            void SendUserMessage()
            {
                if (m_inputBuffer[0] != '\0')
                {
                    Logger::GetInstance().Log(LogLevel::User, m_inputBuffer);
                    
                    m_inputBuffer[0] = '\0';
                    m_filterCacheDirty = true;
                    
                    ImGui::SetKeyboardFocusHere(-1);
                }
            }

            void CopyAllToClipboard()
            {
                if (!m_consolePolicy)
                    return;
                
                const auto& logs = m_consolePolicy->GetLogs();
                std::string clipboardText;
                
                for (const auto& log : logs)
                {
                    if (ShouldShowLevel(log.Level))
                    {
                        clipboardText += "[" + log.Timestamp + "] ["
                                        + LogLevelToString(log.Level) + "] "
                                        + log.Message + "\n";
                    }
                }
                
                ImGui::SetClipboardText(clipboardText.c_str());
            }

            const std::vector<LogEntry>& GetFilteredLogs() const
            {
                if (!m_consolePolicy) 
                {
                    static std::vector<LogEntry> empty;

                    return empty;
                }

                if (m_filterCacheDirty)
                {
                    const auto& allLogs = m_consolePolicy->GetLogs();
                    m_filteredLogs.clear();
                    
                    for (const auto& log : allLogs)
                        if (ShouldShowLevel(log.Level))
                            m_filteredLogs.push_back(log);
                    
                    m_filterCacheDirty = false;
                }
                
                return m_filteredLogs;
            }

            bool ShouldShowLevel(LogLevel level) const
            {
                switch (level)
                {
                    case LogLevel::Trace:    return m_showTrace;
                    case LogLevel::Debug:    return m_showDebug;
                    case LogLevel::Info:     return m_showInfo;
                    case LogLevel::Warning:  return m_showWarning;
                    case LogLevel::Error:    return m_showError;
                    case LogLevel::Critical: return m_showCritical;
                    case LogLevel::User:     return m_showUser;
                    default:                 return true;
                }
            }

            void SetNeedCacheUpdate()
            {
                m_filterCacheDirty = true;
            }
    };
}

#endif
