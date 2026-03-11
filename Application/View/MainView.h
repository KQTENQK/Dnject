#ifndef __MAINVIEW__
#define __MAINVIEW__

#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <imgui.h>
#include "Unii/Unii.h"
#include "Model/MainModel.h"
#include "Model/ProcessBrowserModel.h"
#include "Presenter/ProcessBrowserPresenter.h"

namespace Dnject
{
    class MainView : public Unii::IView
    {
        private:
            int m_selectedProcessIndex;

            MainModel& m_mainModel;
            ProcessBrowserModel& m_processBrowserModel;

            Unii::Event<> m_processRefreshRequested;
            Unii::Event<> m_fileBrowseRequested;
            Unii::Event<> m_injectRequested;

        public:
            MainView
            (
                const std::string& type,
                const std::string& category,
                const std::string& displayName,
                MainModel& mainModel,
                ProcessBrowserModel& processBrowserModel
            ) : Unii::IView(type, category, displayName),
                m_mainModel(mainModel),
                m_processBrowserModel(processBrowserModel)
            {
                m_selectedProcessIndex = -1;

                ImGuiStyle& style = ImGui::GetStyle();
                const ImVec4 windowBackground = ImVec4(0.086f, 0.094f, 0.114f, 1.0f);
                const ImVec4 panelBackground = ImVec4(0.094f, 0.094f, 0.094f, 1.0f);
                const ImVec4 elevatedBackground = ImVec4(0.290f, 0.290f, 0.290f, 1.0f);
                const ImVec4 border = ImVec4(0.180f, 0.196f, 0.239f, 1.0f);
                const ImVec4 textPrimary = ImVec4(0.929f, 0.941f, 0.973f, 1.0f);
                const ImVec4 textSecondary = ImVec4(0.616f, 0.655f, 0.725f, 1.0f);
                const ImVec4 accent = ImVec4(0.192f, 0.741f, 0.443f, 1.0f);
                const ImVec4 accentHover = ImVec4(0.239f, 0.804f, 0.490f, 1.0f);
                const ImVec4 danger = ImVec4(0.839f, 0.357f, 0.357f, 1.0f);
                const ImVec4 inputBackground = ImVec4(0.078f, 0.086f, 0.106f, 1.0f);
                const ImVec4 responseBackground = ImVec4(0.094f, 0.102f, 0.125f, 1.0f);
                const ImVec4 selectedBackground = ImVec4(0.290f, 0.290f, 0.290f, 1.0f);
                const ImVec4 selectedHoverBackground = ImVec4(0.207f, 0.207f, 0.207f, 1.0f);
                const ImVec4 buttonBackground = ImVec4(0.207f, 0.207f, 0.207f, 1.0f);
                const ImVec4 buttonHoverBackground = ImVec4(0.290f, 0.290f, 0.290f, 1.0f);
                const ImVec4 headingText = ImVec4(0.953f, 0.965f, 0.988f, 1.0f);
                const ImVec4 headingLine = ImVec4(0.180f, 0.741f, 0.443f, 1.0f);

                style.WindowRounding = 0.0f;
                style.ChildRounding = 10.0f;
                style.FrameRounding = 8.0f;
                style.GrabRounding = 8.0f;
                style.PopupRounding = 8.0f;
                style.ScrollbarRounding = 8.0f;
                style.FramePadding = ImVec2(18.0f, 7.0f);
                style.ItemSpacing = ImVec2(8.0f, 8.0f);
                style.WindowPadding = ImVec2(18.0f, 12.0f);
                style.WindowBorderSize = 0.0f;
                style.ChildBorderSize = 1.0f;
                style.PopupBorderSize = 1.0f;
                style.FrameBorderSize = 0.0f;
                style.ScrollbarSize = 11.0f;
                style.WindowMenuButtonPosition = ImGuiDir_None;
                style.Colors[ImGuiCol_WindowBg] = windowBackground;
                style.Colors[ImGuiCol_ChildBg] = panelBackground;
                style.Colors[ImGuiCol_Border] = border;
                style.Colors[ImGuiCol_FrameBg] = panelBackground;
                style.Colors[ImGuiCol_FrameBgHovered] = elevatedBackground;
                style.Colors[ImGuiCol_FrameBgActive] = elevatedBackground;
                style.Colors[ImGuiCol_Text] = textPrimary;
                style.Colors[ImGuiCol_TextDisabled] = textSecondary;
                style.Colors[ImGuiCol_Button] = buttonBackground;
                style.Colors[ImGuiCol_ButtonHovered] = buttonHoverBackground;
                style.Colors[ImGuiCol_ButtonActive] = selectedHoverBackground;
                style.Colors[ImGuiCol_Header] = selectedBackground;
                style.Colors[ImGuiCol_HeaderHovered] = selectedHoverBackground;
                style.Colors[ImGuiCol_HeaderActive] = selectedHoverBackground;
                style.Colors[ImGuiCol_Separator] = border;
                style.Colors[ImGuiCol_CheckMark] = accent;
                style.Colors[ImGuiCol_SliderGrab] = accent;
                style.Colors[ImGuiCol_SliderGrabActive] = accentHover;
                style.Colors[ImGuiCol_Tab] = buttonBackground;
                style.Colors[ImGuiCol_TabHovered] = buttonHoverBackground;
                style.Colors[ImGuiCol_TabActive] = selectedBackground;
                style.Colors[ImGuiCol_TitleBg] = windowBackground;
                style.Colors[ImGuiCol_TitleBgActive] = windowBackground;
                style.Colors[ImGuiCol_ScrollbarBg] = panelBackground;
                style.Colors[ImGuiCol_ScrollbarGrab] = elevatedBackground;
                style.Colors[ImGuiCol_ScrollbarGrabHovered] = buttonHoverBackground;
                style.Colors[ImGuiCol_ScrollbarGrabActive] = accent;
                style.Colors[ImGuiCol_TableHeaderBg] = elevatedBackground;
                style.Colors[ImGuiCol_TableBorderStrong] = border;
                style.Colors[ImGuiCol_TableBorderLight] = border;
                style.Colors[ImGuiCol_ResizeGrip] = buttonBackground;
                style.Colors[ImGuiCol_ResizeGripHovered] = buttonHoverBackground;
                style.Colors[ImGuiCol_ResizeGripActive] = accent;
                style.Colors[ImGuiCol_PopupBg] = panelBackground;
            }

            Unii::IEvent<>& ProcessRefreshRequested() { return m_processRefreshRequested; }
            Unii::IEvent<>& FileBrowseRequested() { return m_fileBrowseRequested; }
            Unii::IEvent<>& InjectRequested() { return m_injectRequested; }

            void Draw() override
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);

                ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar
                    | ImGuiWindowFlags_NoResize
                    | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoCollapse
                    | ImGuiWindowFlags_NoSavedSettings
                    | ImGuiWindowFlags_NoBringToFrontOnFocus
                    | ImGuiWindowFlags_NoNavFocus;

                if (ImGui::Begin("Dnject", nullptr, flags))
                    DrawLayout();

                ImGui::End();
            }

        private:
            void DrawLayout()
            {
                const float contentPadding = 24.0f;

                ImDrawList* drawList = ImGui::GetWindowDrawList();
                ImVec2 windowPos = ImGui::GetWindowPos();
                ImVec2 windowSize = ImGui::GetWindowSize();

                drawList->AddRectFilled(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y), IM_COL32(33, 33, 33, 255));
                drawList->AddRectFilled(windowPos, ImVec2(0.0f, 0.0f), IM_COL32(25, 25, 25, 255));

                ImGui::BeginChild("##main_content", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_NoBackground);
                ImGui::TextUnformatted("Dnject");
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(140, 140, 140, 255));

                ImGui::PopStyleColor();
                DrawActionBar();
                ImGui::Spacing();
                DrawDllSection();
                ImGui::Spacing();
                DrawProcessList();
                ImGui::Spacing();
                DrawStatusSection();

                ImGui::EndChild();
            }

            void DrawActionBar()
            {
                if (ImGui::Button("Refresh", ImVec2(110.0f, 0.0f)))
                    m_processRefreshRequested.Invoke();

                ImGui::SameLine();

                if (ImGui::Button("Browse DLL", ImVec2(110.0f, 0.0f)))
                    m_fileBrowseRequested.Invoke();

                ImGui::SameLine();

                if (ImGui::Button("Inject", ImVec2(110.0f, 0.0f)))
                    m_injectRequested.Invoke();

                ImGui::SameLine();
            }

            void DrawDllSection()
            {
                ImGui::TextUnformatted("DLL path");

                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;

                std::string selectedFilePath = converter.to_bytes(m_mainModel.Config().SelectedFilePath);

                if (selectedFilePath.empty())
                    selectedFilePath = "No DLL selected";

                ImGui::PushItemWidth(-1.0f);
                ImGui::InputText("##dll_path", selectedFilePath.data(), selectedFilePath.size() + 1, ImGuiInputTextFlags_ReadOnly);
                ImGui::PopItemWidth();
            }

            void DrawProcessList()
            {
                ImGui::TextUnformatted("Processes");

                if (ImGui::BeginListBox("##processes", ImVec2(-1.0f, 320.0f)))
                {
                    const auto& processes = m_processBrowserModel.Processes();

                    for (int i = 0; i < static_cast<int>(processes.size()); ++i)
                    {
                        bool selected = i == m_selectedProcessIndex;
                        std::wstring wideLabel = processes[i].Name + L" [" + std::to_wstring(processes[i].Id) + L"]";

                        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;

                        std::string label = converter.to_bytes(wideLabel);

                        if (ImGui::Selectable(label.c_str(), selected))
                        {
                            m_selectedProcessIndex = i;
                            m_mainModel.Config().SelectedProcessName = processes[i].Name;
                        }
                    }

                    ImGui::EndListBox();
                }
            }

            void DrawStatusSection()
            {
                ImGui::Separator();
                ImGui::TextUnformatted("Status");
                ImGui::TextWrapped("%ls", m_mainModel.GetStatusMessage().c_str());
            }

            void SyncSelectionFromUi()
            {
                if (m_selectedProcessIndex >= 0 && m_selectedProcessIndex < static_cast<int>(m_processBrowserModel.Processes().size()))
                    m_mainModel.Config().SelectedProcessName = m_processBrowserModel.Processes()[m_selectedProcessIndex].Name;
            }
    };
}

#endif
