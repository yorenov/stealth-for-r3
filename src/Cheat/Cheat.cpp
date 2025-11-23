#include "Cheat/Cheat.h"

#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include "Utils.h"
#include "Features/Aimbot.h"
#include "Features/Combat.h"
#include "Features/Player.h"
#include "Features/Rainbow.h"
#include "Features/Visuals.h"

void
Cheat::Render() {
    if (!Utils::isGTAMenuActive()) {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        pAimbot->Render();
        pVisuals->Render();

        if (pMenu->bOpen)
            pMenu->Render();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }
}

void
Cheat::Update() {
    pAimbot->Update();
    Combat::Update();
    Player::Update();
    CRainbow::Update();
    pMenu->Update();
}
