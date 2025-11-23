#include "Menu/Menu.h"

#include <algorithm>
#include <string>
#include <vector>

#include "cheat/Structs.h"
#include "Cheat/Utils.h"
#include "Cheat/Config/Config.h"
#include "Features/Aimbot.h"

#include "sampapi/0.3.7-R3-1/CGame.h"

CMenu *pMenu;

CMenu::~CMenu() {
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void
CMenu::Update() {
    if (!Utils::isGTAMenuActive()) {
        if (bOpen) {
            showCursor(true);
        } else if (iToggle == 0) {
            showCursor(false);
            bOpen = false;
            iToggle = 3;
        }
    } else if (bOpen) showCursor(false);
}

void
CMenu::Theme() {
    ImGuiStyle &style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.WindowPadding = {8, 8};
    style.WindowMinSize = {32, 32};
    style.WindowRounding = 3.0f;
    style.PopupRounding = 3.0f;
    style.WindowTitleAlign = {0.5f, 0.5f};

    style.FramePadding = {4, 3};
    style.FrameRounding = 3.3f;
    style.ChildRounding = 5.0f;
    style.ItemSpacing = {8, 4};
    style.TouchExtraPadding = {0, 0};
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 0.0f;
    style.ScrollbarSize = 6.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabMinSize = 5.0f;
    style.GrabRounding = 3.3f;
    style.ButtonTextAlign = {0.5f, 0.5f};
    style.DisplayWindowPadding = {22, 22};
    style.DisplaySafeAreaPadding = {4, 4};
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
    style.CurveTessellationTol = 1.f;

    switch (g_Config.g_Style.iTheme) {
        case 0: {
            g_Config.g_Style.vecTheme[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.71f, 0.74f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.13f, 0.16f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_PopupBg] = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.16f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.10f, 0.15f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.13f, 0.17f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_FrameBgActive] = ImVec4(0.07f, 0.08f, 0.13f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ScrollbarBg] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.87f, 0.90f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_SliderGrab] = ImVec4(0.48f, 0.49f, 0.51f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.67f, 0.69f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_Button] = ImVec4(0.09f, 0.10f, 0.15f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.13f, 0.17f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ButtonActive] = ImVec4(0.07f, 0.08f, 0.13f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_Header] = ImVec4(0.29f, 0.34f, 0.43f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_HeaderHovered] = ImVec4(0.21f, 0.24f, 0.31f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_HeaderActive] = ImVec4(0.29f, 0.34f, 0.43f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            g_Config.g_Style.vecTheme[ImGuiCol_SeparatorHovered] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            g_Config.g_Style.vecTheme[ImGuiCol_SeparatorActive] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            g_Config.g_Style.vecTheme[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
            g_Config.g_Style.vecTheme[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            g_Config.g_Style.vecTheme[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            g_Config.g_Style.vecTheme[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TabActive] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
            g_Config.g_Style.vecTheme[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);
            g_Config.g_Style.vecTheme[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
            g_Config.g_Style.vecTheme[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            g_Config.g_Style.vecTheme[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            g_Config.g_Style.vecTheme[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
            break;
        }
        case 1: {
            ImGui::StyleColorsDark();
            break;
        }
        case 2: {
            ImGui::StyleColorsLight();
            break;
        }
        case 3: {
            ImGui::StyleColorsClassic();
            break;
        }
        case 4: {
            g_Config.g_Style.vecTheme[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.71f, 0.74f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.98f);
            g_Config.g_Style.vecTheme[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.98f);
            g_Config.g_Style.vecTheme[ImGuiCol_Border] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.94f);
            g_Config.g_Style.vecTheme[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.94f);
            g_Config.g_Style.vecTheme[ImGuiCol_FrameBgActive] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
            g_Config.g_Style.vecTheme[ImGuiCol_MenuBarBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.94f);
            g_Config.g_Style.vecTheme[ImGuiCol_ScrollbarBg] = ImVec4(0.26f, 0.26f, 0.26f, 0.94f);
            g_Config.g_Style.vecTheme[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.10f, 0.61f, 0.19f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.20f, 0.71f, 0.29f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_SliderGrab] = ImVec4(0.10f, 0.61f, 0.19f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_SliderGrabActive] = ImVec4(0.20f, 0.71f, 0.29f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_Button] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ButtonHovered] = ImVec4(0.10f, 0.61f, 0.19f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.71f, 0.29f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_Header] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_HeaderHovered] = ImVec4(0.10f, 0.61f, 0.19f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.71f, 0.29f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_Separator] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.61f, 0.19f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_SeparatorActive] = ImVec4(0.20f, 0.71f, 0.29f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ResizeGripHovered] = ImVec4(0.10f, 0.61f, 0.19f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_ResizeGripActive] = ImVec4(0.20f, 0.71f, 0.29f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TabActive] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_PlotLinesHovered] = ImVec4(0.10f, 0.61f, 0.19f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.51f, 0.09f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_PlotHistogramHovered] = ImVec4(0.10f, 0.61f, 0.19f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
            g_Config.g_Style.vecTheme[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
            g_Config.g_Style.vecTheme[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            g_Config.g_Style.vecTheme[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            g_Config.g_Style.vecTheme[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            g_Config.g_Style.vecTheme[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
            break;
        }
        default: break;
    }

    for (int i = 0; i < ImGuiCol_COUNT; i++)
        ImGui::GetStyle().Colors[i] = g_Config.g_Style.vecTheme[i];
}

void
CMenu::ActiveTab() {
    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(27 / 255.f, 30 / 255.f, 35 / 255.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_Text));
}

void
CMenu::NormalTab() {
    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Tab));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(27 / 255.f, 30 / 255.f, 35 / 255.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Tab));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_Tab));
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
}

void
CMenu::ActiveButton() {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_Text));
}

void
CMenu::NormalButton() {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
}

void
CMenu::Tooltip(const char *szText) {
    if (strlen(szText) && ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted(szText);
        ImGui::EndTooltip();
    }
}

void
CMenu::Render() {
    ImGui::SetNextWindowSize({600, 424});
    ImGui::SetNextWindowPos({ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f},
                            ImGuiCond_Once,
                            {0.5f, 0.5f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {16, 16});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {1, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {14, -10});

    if (ImGui::Begin("STEALTH",
                     &bOpen,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoScrollbar)) {
        if (iCurrentTab == 0) ActiveTab();
        else NormalTab();
        if (ImGui::Button("Weapon", {0, 25})) iCurrentTab = 0;
        ImGui::PopStyleColor(5);
        ImGui::SameLine();

        if (iCurrentTab == 1) ActiveTab();
        else NormalTab();
        if (ImGui::Button("Player", {0, 25})) iCurrentTab = 1;
        ImGui::PopStyleColor(5);
        ImGui::SameLine();

        if (iCurrentTab == 2) ActiveTab();
        else NormalTab();
        if (ImGui::Button("Visual", {0, 25})) iCurrentTab = 2;
        ImGui::PopStyleColor(5);
        ImGui::SameLine();

        if (iCurrentTab == 3) ActiveTab();
        else NormalTab();
        if (ImGui::Button("Style", {0, 25})) iCurrentTab = 3;
        ImGui::PopStyleColor(5);
        ImGui::SameLine();

        if (iCurrentTab == 4) ActiveTab();
        else NormalTab();
        if (ImGui::Button("Extra", {0, 25})) iCurrentTab = 4;
        ImGui::PopStyleColor(5);
        ImGui::SameLine();

        if (iCurrentTab == 5) ActiveTab();
        else NormalTab();
        if (ImGui::Button("Developer", {0, 25})) iCurrentTab = 5;
        ImGui::PopStyleColor(5);

        ImGui::PopStyleVar(5);
        ImGui::Dummy({0, 7});

        switch (iCurrentTab) {
            case 0: {
                ImGui::Spacing();
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {16, 6});
                ImGui::BeginChild("##Weapons", {0, 50}, true); {
                    ImGui::PushFont(pIcons);

                    if (iCurrentCategory == 0) ActiveButton();
                    else NormalButton();
                    if (ImGui::Button("A", {0, 32})) iCurrentCategory = 0;
                    ImGui::PopStyleColor(4);
                    ImGui::SameLine(105);

                    if (iCurrentCategory == 1) ActiveButton();
                    else NormalButton();
                    if (ImGui::Button("H", {0, 32})) iCurrentCategory = 1;
                    ImGui::PopStyleColor(4);
                    ImGui::SameLine(200);

                    if (iCurrentCategory == 2) ActiveButton();
                    else NormalButton();
                    if (ImGui::Button("c", {0, 32})) iCurrentCategory = 2;
                    ImGui::PopStyleColor(4);
                    ImGui::SameLine(325);

                    if (iCurrentCategory == 3) ActiveButton();
                    else NormalButton();
                    if (ImGui::Button("T", {0, 32})) iCurrentCategory = 3;
                    ImGui::PopStyleColor(4);
                    ImGui::SameLine(445);

                    if (iCurrentCategory == 4) ActiveButton();
                    else NormalButton();
                    if (ImGui::Button("Z", {0, 32})) iCurrentCategory = 4;
                    ImGui::PopStyleColor(4);

                    ImGui::PopFont();
                    ImGui::PopStyleVar();
                    ImGui::EndChild();
                }

                ImGui::BeginGroup();
                ImGui::BeginChild("##Aimbot", {280, 147}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Aimbot"), ImGui::EndMenuBar();
                    ImGui::Checkbox("Aimbot Range", &g_Config.g_Aimbot.bAimbot);
                    ImGui::SameLine(202);
                    ImGui::Button("Settings##Range", {70, 0});
                    if (ImGui::BeginPopupContextItem(nullptr, 0)) {
                        if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Aimbot Range Settings"), ImGui::EndMenuBar();
                        ImGui::Checkbox("Render Range", &g_Config.g_Aimbot.bDrawRange);
                        ImGui::SameLine(150);
                        ImGui::PushItemWidth(70);
                        ImGui::Combo("##RangeStyle", &g_Config.g_Aimbot.iRangeStyle, "Filled\0Outline\0");
                        ImGui::Checkbox("Render Tracer", &g_Config.g_Aimbot.bDrawTracer);
                        ImGui::SameLine(150);
                        ImGui::SliderFloat("##OutlineThickness", &g_Config.g_Aimbot.fOutlineThickness, 1.0f, 5.0f,
                                           "%0.1f");
                        Tooltip("Thickness");
                        ImGui::PopItemWidth();
                        ImGui::PushItemWidth(161);
                        ImGui::ColorEdit4("Range", reinterpret_cast<float *>(&g_Config.g_Aimbot.colorRange),
                                          ImGuiColorEditFlags_AlphaBar);
                        ImGui::PopItemWidth();
                        ImGui::EndPopup();
                    }
                    ImGui::Checkbox("Silent Aimbot", &g_Config.g_Aimbot.bSilent);
                    ImGui::SameLine(202);
                    ImGui::PushItemWidth(70.f);
                    ImGui::SliderFloat("##Accuracy", &g_Config.g_Aimbot.fSilentAccuracy, 20.f, 100.f, "%.0f%%");
                    Tooltip("Silent Recoil");
                    ImGui::PopItemWidth();
                    ImGui::Checkbox("Smooth Aimbot", &g_Config.g_Aimbot.bSmooth);
                    ImGui::SameLine(202);
                    ImGui::Button("Settings##Smooth", {70, 0});
                    if (ImGui::BeginPopupContextItem(nullptr, 0)) {
                        if (ImGui::BeginMenuBar())
                            ImGui::TextUnformatted("Smooth Aimbot Settings"), ImGui::EndMenuBar();
                        ImGui::Checkbox("Aim On Fire", &g_Config.g_Aimbot.bSmoothIsFire);
                        ImGui::Checkbox("Stop On Body", &g_Config.g_Aimbot.bStopOnBody);
                        ImGui::Checkbox("Lock Y Axis", &g_Config.g_Aimbot.bSmoothLockY);
                        ImGui::EndPopup();
                    }
                    ImGui::Checkbox("Pro Aimbot", &g_Config.g_Aimbot.bProAim);
                    ImGui::Checkbox("Triggerbot", &g_Config.g_Aimbot.bTriggerbot);
                    ImGui::SameLine(202);
                    ImGui::Button("Settings##Trigger", {70, 0});
                    if (ImGui::BeginPopupContextItem(nullptr, 0)) {
                        if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Triggerbot Settings"), ImGui::EndMenuBar();
                        ImGui::Checkbox("Delay", &g_Config.g_Aimbot.bTriggerDelay);
                        ImGui::SameLine(150);
                        ImGui::PushItemWidth(70);
                        ImGui::DragInt("##TriggerDelay", &g_Config.g_Aimbot.iTriggerDelay, 1, 0, 500, "%d ms");

                        ImGui::BeginDisabled(!g_Config.g_Aimbot.bAimbot || !g_Config.g_Aimbot.bSilent);
                        ImGui::Checkbox("Sync with Silent", &g_Config.g_Aimbot.bTriggerSync);
                        ImGui::SameLine();
                        ImGui::EndDisabled();

                        if (!g_Config.g_Aimbot.bAimbot || !g_Config.g_Aimbot.bSilent)
                            Tooltip("Enable Range and Silent to use this function!");

                        ImGui::EndPopup();
                    }
                    ImGui::EndChild();
                }
                ImGui::BeginChild("##Filters", {280, 147}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Aimbot Filters"), ImGui::EndMenuBar();
                    ImGui::Checkbox("Team Protect", &g_Config.g_Aimbot.bTeamProtect);
                    ImGui::Checkbox("Lock Through Objects", &g_Config.g_Aimbot.bLockThroughObjects);
                    ImGui::Checkbox("Ignore Max Distance", &g_Config.g_Aimbot.bIgnoreMaxDistance);
                    ImGui::Checkbox("Ignore AFK", &g_Config.g_Aimbot.bIgnoreAFK);
                    ImGui::Checkbox("Ignore Everything", &g_Config.g_Aimbot.bIgnoreEverything);
                    ImGui::EndChild();
                }
                ImGui::EndGroup();
                ImGui::SameLine();
                ImGui::BeginGroup();
                ImGui::BeginChild("##Config", {280, 101}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar())
                        ImGui::TextUnformatted(szCategory[pMenu->iCurrentCategory]),
                                ImGui::EndMenuBar();
                    for (const auto &[fst, snd]: mapWeapons) {
                        if (snd.iCategory != pMenu->iCurrentCategory)
                            continue;

                        ImGui::Checkbox(snd.szName.c_str(), &g_Config.g_Aimbot.bAimbotEnabled[fst]);
                        ImGui::SameLine(202);
                        ImGui::Button(("Config##" + std::to_string(fst)).c_str(), {70, 0});
                        if (ImGui::BeginPopupContextItem(nullptr, 0)) {
                            if (ImGui::BeginMenuBar())
                                ImGui::Text("%s (%d) Configuration",
                                            snd.szName.c_str(),
                                            fst), ImGui::EndMenuBar();
                            g_Config.g_Aimbot.bAimbot
                                ? ImGui::TextUnformatted("Range")
                                : ImGui::TextDisabled("Range");
                            ImGui::SameLine(120);
                            ImGui::SliderInt("##Range", &g_Config.g_Aimbot.iAimbotConfig[fst][RANGE], 1, 200, "%d px");
                            if (ImGui::IsItemActive())
                                g_Config.g_Aimbot.iRangeStyle
                                    ? ImGui::GetForegroundDrawList()->AddCircle(
                                        {pAimbot->vecCrosshair.x, pAimbot->vecCrosshair.y},
                                        static_cast<float>(g_Config.g_Aimbot.iAimbotConfig[fst][RANGE]) * 1.5f,
                                        static_cast<ImColor>(g_Config.g_Aimbot.colorRange),
                                        64,
                                        g_Config.g_Aimbot.fOutlineThickness)
                                    : ImGui::GetForegroundDrawList()->AddCircleFilled(
                                        {pAimbot->vecCrosshair.x, pAimbot->vecCrosshair.y},
                                        static_cast<float>(g_Config.g_Aimbot.iAimbotConfig[fst][RANGE]) * 1.5f,
                                        static_cast<ImColor>(g_Config.g_Aimbot.colorRange),
                                        64);
                            g_Config.g_Aimbot.bSilent
                                ? ImGui::TextUnformatted("Hitchance")
                                : ImGui::TextDisabled("Hitchance");
                            ImGui::SameLine(120);
                            ImGui::SliderInt("##Silent", &g_Config.g_Aimbot.iAimbotConfig[fst][SILENT], 1, 100, "%d%%");
                            g_Config.g_Aimbot.bSmooth
                                ? ImGui::TextUnformatted("Smoothness")
                                : ImGui::TextDisabled("Smoothness");
                            ImGui::SameLine(120);
                            ImGui::SliderInt("##Smooth", &g_Config.g_Aimbot.iAimbotConfig[fst][SMOOTH], 1, 100, "%d%%");
                            ImGui::EndPopup();
                        }
                    }
                    ImGui::EndChild();
                }
                ImGui::BeginChild("##Combat", {280, 193}, true,
                                  ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar); {
                    ImGui::Checkbox("No Reload", &g_Config.g_Combat.bNoReload);
                    ImGui::Checkbox("No Recoil", &g_Config.g_Combat.bNoRecoil);
                    ImGui::EndChild();
                }
                ImGui::EndGroup();
                break;
            }
            case 1: {
                ImGui::Spacing();
                ImGui::BeginChild("##Player", {280, 352}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Player Assistance"), ImGui::EndMenuBar();
                    ImGui::PushItemWidth(70);
                    ImGui::Checkbox("Anti Stun", &g_Config.g_Player.bAntiStun);
                    ImGui::SameLine(202);
                    ImGui::SliderInt("##AntiStun", &g_Config.g_Player.iAntiStunChance, 1, 100);
                    if (ImGui::Checkbox("Air Break", &g_Config.g_Player.bAirBreak))
                        g_Config.g_Player.bMapRun = false;
                    ImGui::SameLine(202);
                    ImGui::SliderInt("##AirBreak", &g_Config.g_Player.iAirBreak, 1, 100);
                    if (ImGui::Checkbox("Map Run", &g_Config.g_Player.bMapRun))
                        g_Config.g_Player.bAirBreak = false;
                    ImGui::SameLine(202);
                    ImGui::SliderInt("##MapRun", &g_Config.g_Player.iMapRun, 1, 100);
                    ImGui::PopItemWidth();
                    ImGui::EndChild();
                }
                ImGui::SameLine();
                ImGui::BeginChild("##Sync", {280, 170}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Sync Modifications"), ImGui::EndMenuBar();
                    ImGui::Checkbox("Upside Mode", &g_Config.g_Player.bUpsideMode);
                    ImGui::Checkbox("Invert Walk", &g_Config.g_Player.bInvertWalk);
                    ImGui::Checkbox("Shake Mode", &g_Config.g_Player.bShakeMode);
                    ImGui::Checkbox("Invalid Aim Z", &g_Config.g_Player.bInvalidAimZ);
                    ImGui::PushItemWidth(70);
                    ImGui::Checkbox("Fake Lag", &g_Config.g_Player.bFakeLag);
                    ImGui::SameLine(202);
                    ImGui::DragInt("##FakeLag", &g_Config.g_Player.iFakeLag, 1, 1, 500, "%d ms");
                    ImGui::PopItemWidth();
                    ImGui::EndChild();
                }
                break;
            }
            case 2: {
                ImGui::Spacing();
                ImGui::BeginGroup();
                ImGui::BeginChild("##PlayerESP", {280, 193}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Player ESP"), ImGui::EndMenuBar();
                    ImGui::Checkbox("ESP Name", &g_Config.g_Visuals.bNameTagsESP);
                    ImGui::Checkbox("ESP Box", &g_Config.g_Visuals.bBoxESP);
                    ImGui::SameLine(202);
                    ImGui::PushItemWidth(50.f);
                    ImGui::Combo("##RangeStyle", &g_Config.g_Visuals.iBoxStyle, "2D\0" "3D\0");
                    ImGui::PopItemWidth();
                    ImGui::SameLine(0.0f, 1.0f);
                    ImGui::Checkbox("##BoxFilled", &g_Config.g_Visuals.bFilledBox);
                    Tooltip("Filled");
                    ImGui::Checkbox("ESP Chams", &g_Config.g_Visuals.bChamsESP);
                    ImGui::Checkbox("ESP Snipeline", &g_Config.g_Visuals.bSnipelineESP);
                    ImGui::SameLine(202);
                    ImGui::PushItemWidth(70.f);
                    ImGui::Combo("##SnipelineStyle", &g_Config.g_Visuals.iSnipelineStyle, "Above\0Down\0");
                    ImGui::PopItemWidth();
                    ImGui::Checkbox("ESP Bones", &g_Config.g_Visuals.bBoneESP);
                    ImGui::Checkbox("ESP Weapon", &g_Config.g_Visuals.bWeaponESP);
                    ImGui::Checkbox("ESP Distance", &g_Config.g_Visuals.bDistanceESP);
                    ImGui::EndChild();
                }
                ImGui::BeginChild("##ESPSettings", {280, 155}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("ESP Settings"), ImGui::EndMenuBar();
                    ImGui::TextUnformatted("ESP Thickness");
                    ImGui::SameLine(202);
                    ImGui::PushItemWidth(70.f);
                    ImGui::SliderFloat("##Thickness", &g_Config.g_Visuals.fESPThickness, 1.0f, 3.0f, "%0.1f");
                    ImGui::PopItemWidth();
                    ImGui::Checkbox("Disable for Teammates", &g_Config.g_Visuals.bESPTeammates);
                    ImGui::Checkbox("Use Custom Colors", &g_Config.g_Visuals.bESPCustomColors);
                    ImGui::PushItemWidth(160.f);
                    ImGui::ColorEdit4("Visible", reinterpret_cast<float *>(&g_Config.g_Visuals.colorVisible),
                                      ImGuiColorEditFlags_AlphaBar);
                    ImGui::ColorEdit4("Not Visible",
                                      reinterpret_cast<float *>(&g_Config.g_Visuals.colorNotVisible),
                                      ImGuiColorEditFlags_AlphaBar);
                    ImGui::PopItemWidth();
                    ImGui::EndChild();
                }
                ImGui::EndGroup();
                ImGui::SameLine();
                ImGui::BeginGroup();
                ImGui::BeginChild("##Visuals", {280, 201}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Visuals"), ImGui::EndMenuBar();
                    ImGui::Checkbox("Aspect Ratio", &g_Config.g_Visuals.bAspectRatio);
                    ImGui::SameLine(202);
                    ImGui::PushItemWidth(70);
                    ImGui::SliderFloat("##AspectRatio", &g_Config.g_Visuals.fAspectRatio, 0.5f, 2.0f, "%0.1f");
                    ImGui::PopItemWidth();
                    ImGui::Checkbox("Damage Informer", &g_Config.g_Visuals.bDMGInformer);
                    ImGui::SameLine(202);
                    ImGui::Button("Settings##DMG", {70, 0});
                    if (ImGui::BeginPopupContextItem(nullptr, 0)) {
                        if (ImGui::BeginMenuBar())
                            ImGui::TextUnformatted("Damage Informer Settings"), ImGui::EndMenuBar();
                        ImGui::TextUnformatted("Font Size");
                        ImGui::SameLine(150);
                        ImGui::PushItemWidth(70);
                        ImGui::SliderInt("##DMGFont", &g_Config.g_Visuals.iFontSize, 7, 20, "%d px");
                        ImGui::TextUnformatted("Time on screen");
                        ImGui::SameLine(150);
                        ImGui::PushItemWidth(70);
                        ImGui::DragFloat("##DMGTick", &g_Config.g_Visuals.fDMGTick, 0.1f, 0.1f, 10.0f, "%.1f s");
                        ImGui::PushItemWidth(161);
                        ImGui::ColorEdit4("Text", reinterpret_cast<float *>(&g_Config.g_Visuals.colorDMG),
                                          ImGuiColorEditFlags_AlphaBar);
                        ImGui::PopItemWidth();
                        ImGui::EndPopup();
                    }
                    ImGui::EndChild();
                }
                ImGui::EndGroup();
                break;
            }
            case 3: {
                ImGui::Spacing();
                ImGui::BeginGroup();
                ImGui::BeginChild("##Theme", {280, 352}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Theme"), ImGui::EndMenuBar();
                    if (ImGui::Button("Set Default Theme", {259, 23}))
                        Theme();

                    for (int i = 0; i < ImGuiCol_COUNT; i++) {
                        ImGui::PushID(i);
                        ImGui::Text(ImGui::GetStyleColorName(i));
                        ImGui::SameLine(176);
                        ImGui::PushItemWidth(90);
                        ImGui::ColorEdit4("##Color",
                                          reinterpret_cast<float *>(&ImGui::GetStyle().Colors[i]),
                                          ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_AlphaBar);
                        ImGui::PopItemWidth();

                        if (ImGui::GetStyle().Colors[i].x != g_Config.g_Style.vecTheme[i].x ||
                            ImGui::GetStyle().Colors[i].y != g_Config.g_Style.vecTheme[i].y ||
                            ImGui::GetStyle().Colors[i].z != g_Config.g_Style.vecTheme[i].z ||
                            ImGui::GetStyle().Colors[i].w != g_Config.g_Style.vecTheme[i].w) {
                            g_Config.g_Style.vecTheme[i] = ImGui::GetStyle().Colors[i];
                        }

                        ImGui::PopID();
                    }

                    ImGui::EndChild();
                    ImGui::EndGroup();
                }
                ImGui::SameLine();
                ImGui::BeginChild("##Styles", {280, 352}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Styles"), ImGui::EndMenuBar();
                    ImGui::TextUnformatted("Menu Colors");
                    ImGui::SameLine(177);
                    ImGui::PushItemWidth(95);
                    if (ImGui::Combo("##MenuColors", &g_Config.g_Style.iTheme,
                                     "Default\0Dark\0Light\0Classic\0Green\0"))
                        Theme();

                    ImGui::TextUnformatted("Checkbox Style");
                    ImGui::SameLine(177);
                    ImGui::Combo("##CheckboxStyle", &g_Config.g_Style.iCheckbox, "Checkmark\0Filled\0");
                    ImGui::PopItemWidth();
                    ImGui::EndChild();
                }
                break;
            }
            case 4: {
                ImGui::Spacing();
                ImGui::BeginGroup();
                ImGui::BeginChild("##Extra", {280, 194}, true, ImGuiWindowFlags_MenuBar); {
                    // if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Extra"), ImGui::EndMenuBar();
                    // if (ImGui::Button("Unload", {70, 0}))
                    // 	ImGui::OpenPopup("Unload");
                    // 	if (ImGui::BeginPopupModal("Unload",
                    // 	                           NULL,
                    // 	                           ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                    // 	                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                    // 		ImGui::Text("(!) Close any recording software before press Unload!");
                    // 		ImGui::Dummy({0, 20});
                    // 		if (ImGui::Button("Unload", ImVec2(160.f, 20.f))) Cheat::Unload();
                    // 		ImGui::SameLine();
                    // 		if (ImGui::Button("Close", ImVec2(160.5f, 20.f))) ImGui::CloseCurrentPopup();
                    // 		ImGui::EndPopup();
                    // }
                    ImGui::EndChild();
                }
                ImGui::BeginChild("##nush 1", {280, 160}, true, ImGuiWindowFlags_MenuBar); {
                    ImGui::EndChild();
                }
                ImGui::EndGroup();

                ImGui::SameLine();

                ImGui::BeginChild("##Configs", {280, 179}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Configs"), ImGui::EndMenuBar();
                    ImGui::Columns(2, nullptr, false);
                    ImGui::SetColumnOffset(1, 170);
                    ImGui::PushItemWidth(160);

                    if (ImGui::Button("Reload configs", {160, 25}))
                        pConfig->listConfigs();

                    auto &vecConfigs = pConfig->getConfigs();
                    static int iCurrentConfig = -1;

                    if (static_cast<std::size_t>(iCurrentConfig) >= vecConfigs.size())
                        iCurrentConfig = -1;

                    static std::string szBuffer;

                    if (ImGui::ListBox("##configs",
                                       &iCurrentConfig,
                                       [](void *data,
                                          const int idx,
                                          const char **out_text) {
                                           const auto &vector = *static_cast<std::vector<std::string> *>(data);
                                           *out_text = vector[idx].c_str();
                                           return true;
                                       },
                                       &vecConfigs,
                                       static_cast<int>(vecConfigs.size()),
                                       5) && iCurrentConfig != -1)
                        szBuffer = vecConfigs[iCurrentConfig];

                    ImGui::PushID(0);
                    if (ImGui::InputTextWithHint("##config_search",
                                                 "Name",
                                                 szBuffer.data(),
                                                 szBuffer.capacity() + 1,
                                                 ImGuiInputTextFlags_EnterReturnsTrue)) {
                        if (iCurrentConfig != -1)
                            pConfig->Rename(iCurrentConfig, szBuffer.c_str());
                    }
                    ImGui::PopID();
                    ImGui::NextColumn();

                    ImGui::PushItemWidth(100);

                    if (ImGui::Button("Create", {95, 25}))
                        pConfig->Add(szBuffer.c_str());

                    if (ImGui::Button("Reset", {95, 25}))
                        ImGui::OpenPopup("Config to reset");

                    if (ImGui::BeginPopup("Config to reset")) {
                        static const char *szConfigs[]{
                            "Whole",
                            "Aimbot",
                            "Combat",
                            "Player",
                            "Visuals",
                            "Style",
                            "Hotkeys"
                        };
                        for (int i = 0; i < IM_ARRAYSIZE(szConfigs); i++) {
                            if (i == 1) ImGui::Separator();
                            if (ImGui::Selectable(szConfigs[i])) {
                                switch (i) {
                                    case 0: CConfig::Reset();
                                        Theme();
                                        break;
                                    case 1: g_Config.g_Aimbot = {};
                                        break;
                                    case 2: g_Config.g_Combat = {};
                                        break;
                                    case 3: g_Config.g_Player = {};
                                        break;
                                    case 4: g_Config.g_Visuals = {};
                                        break;
                                    case 5: g_Config.g_Style = {};
                                        Theme();
                                        break;
                                    case 6: g_Config.g_Hotkeys = {};
                                        break;
                                    default: break;
                                }
                            }
                        }
                        ImGui::EndPopup();
                    }
                    if (iCurrentConfig != -1) {
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
                        if (ImGui::Button("Load", {95, 25})) {
                            pConfig->Load(iCurrentConfig);
                            for (int i = 0; i < ImGuiCol_COUNT; i++)
                                ImGui::GetStyle().Colors[i] = g_Config.g_Style.vecTheme[i];
                        }
                        if (ImGui::Button("Save", {95, 25}))
                            pConfig->Save(iCurrentConfig);
                        if (ImGui::Button("Delete", {95, 25})) {
                            pConfig->Remove(iCurrentConfig);
                            iCurrentConfig = -1;
                            szBuffer.clear();
                        }
                    }
                    ImGui::Columns(1);
                    ImGui::EndChild();
                }
                break;
            }
            case 5: {
                ImGui::Spacing();
                ImGui::BeginGroup();
                ImGui::BeginChild("##Damager", {280, 194}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Damager"), ImGui::EndMenuBar();
                    ImGui::PushItemWidth(70.f);
                    ImGui::Checkbox("Damager", &g_Config.g_Developer.bDamager);
                    ImGui::Checkbox("Delay", &g_Config.g_Developer.bDelay);
                    ImGui::SameLine(202);
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {1, 0});
                    ImGui::InputInt("##Delay", &g_Config.g_Developer.iDelay);
                    ImGui::PopStyleVar();
                    ImGui::Checkbox("Team Protect", &g_Config.g_Developer.bTeamProtect);
                    ImGui::Checkbox("Send Bullet Data", &g_Config.g_Developer.bSendBulletData);
                    ImGui::Checkbox("Teleport To Player", &g_Config.g_Developer.bTeleportToPlayer);
                    ImGui::Checkbox("Custom Weapon", &g_Config.g_Developer.bCustomWeapon);
                    ImGui::SameLine(202);
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {1, 0});
                    ImGui::InputInt("##WeaponID", &g_Config.g_Developer.iWeaponID);
                    ImGui::PopStyleVar();
                    ImGui::Checkbox("Custom Damage", &g_Config.g_Developer.bCustomDamage);
                    ImGui::SameLine(202);
                    ImGui::InputFloat("##Damage", &g_Config.g_Developer.fDamage);
                    ImGui::PopItemWidth();
                    ImGui::EndChild();
                }
                ImGui::BeginChild("##SAMPCAC", {280, 160}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("SAMPCAC"), ImGui::EndMenuBar();

                    ImGui::EndChild();
                }
                ImGui::EndGroup();
                ImGui::SameLine();
                ImGui::BeginChild("##Developer", {280, 358}, true, ImGuiWindowFlags_MenuBar); {
                    if (ImGui::BeginMenuBar()) ImGui::TextUnformatted("Developer"), ImGui::EndMenuBar();

                    ImGui::EndChild();
                }
                break;
            }
            default: break;
        }
        ImGui::End();
    }
}
