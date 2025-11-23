#include "Features/Visuals.h"

#include <common.h>
#include <d3dx9core.h>

#include "Cheat/Utils.h"
#include "Cheat/Config/Config.h"
#include "imgui/imgui_render.h"
#include "sampapi/0.3.7-R3-1/CDeathWindow.h"
#include "sampapi/0.3.7-R3-1/CNetGame.h"
#include "SAMP_disabled/SAMP.h"
#include "D3D/D3DHook.h"
#include <ePedBones.h>
#include <CWorld.h>
#include <CCamera.h>
#include <CMenuManager.h>

CVisuals *pVisuals;

float
__cdecl CVisuals::hkCalculateAspectRatio() {
    if (g_Config.g_Visuals.bAspectRatio && g_Config.g_Visuals.fAspectRatio >= 0.2f && g_Config.g_Visuals.fAspectRatio <=
                                                                                      2.0f &&
        (!FrontEndMenuManager.m_bPrefsUseWideScreen || !TheCamera.m_bWideScreenOn)) {
        pSecure->Write(0xC3EFA4, (float) g_Config.g_Visuals.fAspectRatio);
        return g_Config.g_Visuals.fAspectRatio;
    }

    return pVisuals->oCalculateAspectRatio();
}

void
CVisuals::Render() {
    if (g_Config.g_Visuals.bNameTagsESP || g_Config.g_Visuals.bWeaponESP || g_Config.g_Visuals.bSnipelineESP || g_Config
            .g_Visuals.bBoxESP || g_Config.g_Visuals.bBoneESP || g_Config.g_Visuals.bChamsESP || g_Config.g_Visuals.
            bDistanceESP) {
        auto *const p_player_pool = sampapi::v037r3::RefNetGame()->GetPlayerPool();
        const auto local_player_color = p_player_pool->GetLocalPlayer()->GetColorAsRGBA();

        for (int i = 0; i < SAMP_MAX_PLAYERS; i++) {
            auto *const p_remote_player = p_player_pool->GetPlayer(i);

            if (p_remote_player == nullptr || !p_remote_player->DoesExist())
                continue;

            if (g_Config.g_Visuals.bESPTeammates && p_remote_player->GetColorAsRGBA() == local_player_color)
                continue;

            auto *const p_remote_ped = p_remote_player->m_pPed->m_pGamePed;
            if (p_remote_ped == nullptr)
                continue;

            if (g_Config.g_Visuals.bChamsESP)
                ESP_Chams(i, p_remote_ped);
            if (g_Config.g_Visuals.bSnipelineESP)
                ESP_Snipeline(i, p_remote_ped);
            if (g_Config.g_Visuals.bNameTagsESP || g_Config.g_Visuals.bWeaponESP)
                ESP_NameTags(i, p_remote_ped);
            if (g_Config.g_Visuals.bBoxESP && p_remote_ped->IsAlive())
                ESP_Box(i, p_remote_ped);
            if (g_Config.g_Visuals.bBoneESP)
                ESP_Bones(i, p_remote_ped);
            if (g_Config.g_Visuals.bDistanceESP)
                ESP_Distance(i, p_remote_ped);
        }
    }

    if (g_Config.g_Visuals.bDMGInformer) {
        for (auto &i: g_DMGInformer) {
            if (i.dwTick >= GetTickCount64()) {
                CVector vecScreen;
                i.vecPos.z += 0.007f;

                Utils::CalcScreenCoors(&i.vecPos, &vecScreen);

                if (vecScreen.z < 1.f)
                    continue;

                char szDamage[16];
                sprintf(szDamage, "%0.1f", fWeaponDamage[i.iWeapon]);
                CRender::DrawString(pRender->pESPFont,
                                    szDamage,
                                    vecScreen,
                                    static_cast<float>(g_Config.g_Visuals.iFontSize),
                                    g_Config.g_Visuals.colorDMG,
                                    true);
            } else i = {};
        }

        const auto colorHitmarker = ImColor(255, 255, 255, static_cast<int>(pVisuals->fHitmarkerAlpha * 255));

        if (pVisuals->fHitmarkerAlpha >= 0.02f) {
            CRender::DrawLine(CVector(pAimbot->vecCrosshair.x - 10, pAimbot->vecCrosshair.y - 10, 0),
                              CVector(pAimbot->vecCrosshair.x - 5, pAimbot->vecCrosshair.y - 5, 0), colorHitmarker,
                              1.1f);
            CRender::DrawLine(CVector(pAimbot->vecCrosshair.x - 10, pAimbot->vecCrosshair.y + 10, 0),
                              CVector(pAimbot->vecCrosshair.x - 5, pAimbot->vecCrosshair.y + 5, 0), colorHitmarker,
                              1.1f);
            CRender::DrawLine(CVector(pAimbot->vecCrosshair.x + 10, pAimbot->vecCrosshair.y - 10, 0),
                              CVector(pAimbot->vecCrosshair.x + 5, pAimbot->vecCrosshair.y - 5, 0), colorHitmarker,
                              1.1f);
            CRender::DrawLine(CVector(pAimbot->vecCrosshair.x + 10, pAimbot->vecCrosshair.y + 10, 0),
                              CVector(pAimbot->vecCrosshair.x + 5, pAimbot->vecCrosshair.y + 5, 0), colorHitmarker,
                              1.1f);
            pVisuals->fHitmarkerAlpha -= 0.02f;
        }
    }
}

bool
CVisuals::isPlayerVisible(const int iPlayerID,
                          const bool bDistance) {
    auto *const p_remote_ped = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetPlayer(iPlayerID)->m_pPed->m_pGamePed;
    const CMatrix matEnt = *p_remote_ped->GetMatrix();
    CVector vecTarget = *Utils::getBonePosition(p_remote_ped, BONE_HEAD, &vecTarget);

    if (!CWorld::GetIsLineOfSightClear(*TheCamera.GetGameCamPosition(),
                                       vecTarget,
                                       true,
                                       false,
                                       false,
                                       true,
                                       true,
                                       false,
                                       false)
        || bDistance && Utils::getDistanceFromCamera(matEnt.pos) > sampapi::v037r3::RefNetGame()->m_pSettings->
            m_fNameTagsDrawDist)
        return false;
    return true;
}

ImColor
CVisuals::getESPColor(const int iPlayerID) {
    ImColor colorESP;
    if (g_Config.g_Visuals.bESPCustomColors)
        colorESP = isPlayerVisible(iPlayerID, false)
                   ? ImColor(g_Config.g_Visuals.colorVisible)
                   : ImColor(g_Config.g_Visuals.colorNotVisible);
    else colorESP = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetPlayer(iPlayerID)->GetColorAsRGBA();

    return colorESP;
}

void
CVisuals::ESP_NameTags(const int iPlayerID,
                       CPed *pPed) {
    if (isPlayerVisible(iPlayerID, true))
        return;

    CVector vecHead;
    CVector vecHeadScreen;

    Utils::getBonePosition(pPed, BONE_HEAD2, &vecHead);
    vecHead.z += 0.25f;
    Utils::CalcScreenCoors(&vecHead, &vecHeadScreen);
    if (vecHeadScreen.z < 1.f)
        return;

    if (g_Config.g_Visuals.bNameTagsESP) {
        auto *const p_player_pool = sampapi::v037r3::RefNetGame()->GetPlayerPool();
        auto *const p_remote_player = p_player_pool->GetPlayer(iPlayerID);

        if (p_remote_player->m_nStatus == sampapi::v037r3::CRemotePlayer::PlayerStatus::PLAYER_STATUS_TIMEOUT)
            CRender::DrawString(pRender->pESPFont,
                                "AFK",
                                CVector(vecHeadScreen.x - 40, vecHeadScreen.y - 16.f, 0),
                                12.f,
                                0xFF808080,
                                true);

        char szName[32];
        sprintf(szName, "%s (%d)", p_player_pool->GetName(iPlayerID), iPlayerID);
        CRender::DrawString(pRender->pESPFont,
                            szName,
                            CVector(vecHeadScreen.x, vecHeadScreen.y - 30.0f, 0),
                            15.f,
                            p_remote_player->GetColorAsRGBA(),
                            true);

        float fHealth = p_remote_player->m_fReportedHealth;
        float fArmor = p_remote_player->m_fReportedArmour;

        if (fHealth > 100.0f)
            fHealth = 100.0f;

        fHealth *= 40.f / 100.0f;
        fHealth -= (40.f / 2.f);

        constexpr ImColor colorHealthBar{0.9f, 0.f, 0.f, 1.f};
        constexpr ImColor colorHealthBarBG{0.5f, 0.f, 0.f, 0.f};

        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(vecHeadScreen.x - 21.f,
                                                             (fArmor > 0.0f)
                                                             ? vecHeadScreen.y - 6.f
                                                             : vecHeadScreen.y - 14.f),
                                                      ImVec2(vecHeadScreen.x + 21.f,
                                                             (fArmor > 0.0f)
                                                             ? vecHeadScreen.y
                                                             : vecHeadScreen.y - 8.f),
                                                      0xFF000000);
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(vecHeadScreen.x - 20.f,
                                                             (fArmor > 0.0f)
                                                             ? vecHeadScreen.y - 5.f
                                                             : vecHeadScreen.y - 13.f),
                                                      ImVec2(vecHeadScreen.x + 20.f,
                                                             (fArmor > 0.0f)
                                                             ? vecHeadScreen.y - 1.f
                                                             : vecHeadScreen.y - 9.f),
                                                      ImColor(colorHealthBarBG.Value.z,
                                                              colorHealthBarBG.Value.y,
                                                              colorHealthBarBG.Value.x,
                                                              colorHealthBarBG.Value.w));
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(vecHeadScreen.x - 20.f,
                                                             (fArmor > 0.0f)
                                                             ? vecHeadScreen.y - 5.f
                                                             : vecHeadScreen.y - 13.f),
                                                      ImVec2(vecHeadScreen.x + fHealth,
                                                             (fArmor > 0.0f)
                                                             ? vecHeadScreen.y - 1.f
                                                             : vecHeadScreen.y - 9.f),
                                                      ImColor(colorHealthBar.Value.z,
                                                              colorHealthBar.Value.y,
                                                              colorHealthBar.Value.x,
                                                              colorHealthBar.Value.w));

        if (fArmor > 0.0f) {
            constexpr ImColor colorArmorBar{0.9f, 0.9f, 0.9f, 1.f};
            constexpr ImColor colorArmorBarBG{0.5f, 0.5f, 0.5f, 0.f};

            if (fArmor > 100.0f)
                fArmor = 100.0f;

            fArmor *= 40.f / 100.0f;
            fArmor -= (40.f / 2);
            ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(vecHeadScreen.x - 21.f, vecHeadScreen.y - 14.f),
                                                          ImVec2(vecHeadScreen.x + 21.f, vecHeadScreen.y - 8.f),
                                                          0xFF000000);
            ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(vecHeadScreen.x - 20.f, vecHeadScreen.y - 13.f),
                                                          ImVec2(vecHeadScreen.x + 20.f, vecHeadScreen.y - 9.f),
                                                          ImColor(colorArmorBarBG.Value.z,
                                                                  colorArmorBarBG.Value.y,
                                                                  colorArmorBarBG.Value.x,
                                                                  colorArmorBarBG.Value.w));
            ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(vecHeadScreen.x - 20.f, vecHeadScreen.y - 13.f),
                                                          ImVec2(vecHeadScreen.x + fArmor, vecHeadScreen.y - 9.f),
                                                          ImColor(colorArmorBar.Value.z,
                                                                  colorArmorBar.Value.y,
                                                                  colorArmorBar.Value.x,
                                                                  colorArmorBar.Value.w));
        }
    }

    if (g_Config.g_Visuals.bWeaponESP) {
        auto *const ref_death_window = sampapi::v037r3::RefDeathWindow();
        ID3DXSprite *pSprite = ref_death_window->m_pSprite;
        ID3DXFont *m_pWeaponFont = ref_death_window->m_pWeaponFont1;
        RECT rect = {
                static_cast<LONG>(vecHeadScreen.x) + 25,
                static_cast<LONG>(vecHeadScreen.y) - 16,
                static_cast<LONG>(vecHeadScreen.x) + 1,
                static_cast<LONG>(vecHeadScreen.y) + 1
        };

        pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
        m_pWeaponFont->DrawTextA(pSprite, "G", -1, &rect, DT_NOCLIP | DT_LEFT, 0xFF000000);
        m_pWeaponFont->DrawTextA(pSprite,
                                 ref_death_window->GetWeaponSpriteId(
                                         FindPlayerPed()->m_aWeapons[FindPlayerPed()->m_nActiveWeaponSlot].m_eWeaponType),
                                 -1,
                                 &rect,
                                 DT_NOCLIP | DT_LEFT,
                                 0xFFFFFFFF);
        pSprite->End();
    }
}

void CVisuals::ESP_Snipeline(const int iPlayerID,
                        CPed *pPed) {
    CVector vecTarget = pPed->GetPosition(), vecTargetScreen;
    g_Config.g_Visuals.iSnipelineStyle
    ? vecTarget.z -= 1.1f
    : vecTarget.z += 1.0f;
    Utils::CalcScreenCoors(&vecTarget, &vecTargetScreen);
    if (vecTargetScreen.z < 1.f)
        return;

    CRender::DrawLine(vecTargetScreen,
                      CVector(ImGui::GetIO().DisplaySize.x / 2.f,
                              g_Config.g_Visuals.iSnipelineStyle
                              ? ImGui::GetIO().DisplaySize.y
                              : 0,
                              0),
                      getESPColor(iPlayerID),
                      g_Config.g_Visuals.fESPThickness);
}

void
CVisuals::ESP_Box(const int iPlayerID,
                  CPed *pPed) {
    const CVector vecTarget = pPed->GetPosition();
    CVector vecTargetScreen;
    Utils::CalcScreenCoors(&vecTarget, &vecTargetScreen);

    CVector vecHead = vecTarget, vecFoot = vecTarget, vecHeadScreen, vecFootScreen;
    vecHead.z += 1.0f, vecFoot.z -= 1.1f;

    Utils::CalcScreenCoors(&vecHead, &vecHeadScreen);
    Utils::CalcScreenCoors(&vecFoot, &vecFootScreen);

    if (vecTargetScreen.z < 1.f || vecHeadScreen.z < 1.f || vecFootScreen.z < 1.f)
        return;

    const ImColor colorESP = getESPColor(iPlayerID);

    if (!g_Config.g_Visuals.iBoxStyle) {
        const float fWidth = abs((vecFootScreen.y - vecHeadScreen.y) / 4.f);
        const ImColor colorVisible = isPlayerVisible(iPlayerID, false)
                                     ? ImColor(g_Config.g_Visuals.colorVisible)
                                     : ImColor(g_Config.g_Visuals.colorNotVisible);
        if (g_Config.g_Visuals.bFilledBox)
            CRender::DrawRectFilled(CVector(vecHeadScreen.x - fWidth, vecHeadScreen.y, 0),
                                    CVector(vecFootScreen.x + fWidth, vecFootScreen.y, 0),
                                    ImColor(colorVisible.Value.x, colorVisible.Value.y, colorVisible.Value.z, 0.35f));
        CRender::DrawRect(CVector(vecHeadScreen.x - fWidth, vecHeadScreen.y, 0),
                          CVector(vecFootScreen.x + fWidth, vecFootScreen.y, 0),
                          colorESP,
                          g_Config.g_Visuals.fESPThickness);
    } else {
        const CVector vecMin = {-0.35f, -0.35f, -1.1f};
        const CVector vecMax = {0.35f, 0.35f, 1.0f};
        CRender::DrawBoundingBox(vecTarget,
                                 vecMin,
                                 vecMax,
                                 pPed->m_fCurrentRotation,
                                 colorESP,
                                 g_Config.g_Visuals.fESPThickness,
                                 g_Config.g_Visuals.bFilledBox,
                                 isPlayerVisible(iPlayerID, false)
                                 ? ImColor(g_Config.g_Visuals.colorVisible)
                                 : ImColor(g_Config.g_Visuals.colorNotVisible));
    }
}

void
CVisuals::ESP_Bones(const int iPlayerID,
                    CPed *pPed) {
    CVector vecBone[55];
    for (int iBone = BONE_PELVIS1; iBone <= BONE_RIGHTFOOT; iBone++) {
        switch (iBone) {
            case 5:
            case 4:
            case 22:
            case 32:
            case 23:
            case 33:
            case 24:
            case 34:
            case 25:
            case 26:
            case 35:
            case 36:
            case 3:
            case 2:
            case 52:
            case 42:
            case 53:
            case 43:
            case 54:
            case 44:
                Utils::getBonePosition(pPed, static_cast<ePedBones>(iBone), &vecBone[iBone]);
                break;
            default:
                break;
        }
    }

    //CVector vecHeadScreen;
    //Utils::CalcScreenCoors(&vecBone[BONE_HEAD2], &vecHeadScreen);
    //if (vecHeadScreen.z < 1.0f)
    //	return;

    const ImColor colorESP = getESPColor(iPlayerID);
    //float fDistance = 1.f / (Utils::getDistanceFromCamera(vecBone[BONE_HEAD2]) + 5.f) * 110.f;
    //pRender->DrawCircleFilled(vecHeadScreen, fDistance, colorESP);

    CRender::D3DLine(vecBone[BONE_NECK], vecBone[BONE_UPPERTORSO], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_UPPERTORSO], vecBone[BONE_RIGHTSHOULDER], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_UPPERTORSO], vecBone[BONE_LEFTSHOULDER], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_LEFTSHOULDER], vecBone[BONE_LEFTELBOW], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_RIGHTSHOULDER], vecBone[BONE_RIGHTELBOW], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_LEFTELBOW], vecBone[BONE_LEFTWRIST], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_RIGHTELBOW], vecBone[BONE_RIGHTWRIST], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_LEFTWRIST], vecBone[BONE_LEFTHAND], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_LEFTHAND], vecBone[BONE_LEFTTHUMB], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_RIGHTWRIST], vecBone[BONE_RIGHTHAND], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_RIGHTHAND], vecBone[BONE_RIGHTTHUMB], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_UPPERTORSO], vecBone[BONE_SPINE1], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_SPINE1], vecBone[BONE_PELVIS], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_PELVIS], vecBone[BONE_RIGHTKNEE], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_PELVIS], vecBone[BONE_LEFTKNEE], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_RIGHTKNEE], vecBone[BONE_RIGHTANKLE], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_LEFTKNEE], vecBone[BONE_LEFTANKLE], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_RIGHTANKLE], vecBone[BONE_RIGHTFOOT], colorESP, g_Config.g_Visuals.fESPThickness);
    CRender::D3DLine(vecBone[BONE_LEFTANKLE], vecBone[BONE_LEFTFOOT], colorESP, g_Config.g_Visuals.fESPThickness);
}

void
CVisuals::ESP_Chams(const int iPlayerID,
                    CPed *pPed) {
    if (!pPed->GetIsOnScreen())
        return;

    DWORD dwCONSTANT, dwARG0, dwARG1, dwARG2;
    const ImColor colorESP = getESPColor(iPlayerID);

    pD3DHook->pD3DDevice->GetTextureStageState(0, D3DTSS_CONSTANT, &dwCONSTANT);
    pD3DHook->pD3DDevice->GetTextureStageState(0, D3DTSS_COLORARG0, &dwARG0);
    pD3DHook->pD3DDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwARG1);
    pD3DHook->pD3DDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &dwARG2);

    pD3DHook->pD3DDevice->SetTextureStageState(0,
                                               D3DTSS_CONSTANT,
                                               ImColor(colorESP.Value.z,
                                                       colorESP.Value.y,
                                                       colorESP.Value.x,
                                                       colorESP.Value.w));
    pD3DHook->pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG0, D3DTA_CONSTANT);
    pD3DHook->pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CONSTANT);
    pD3DHook->pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CONSTANT);

    pPed->Render();

    pD3DHook->pD3DDevice->SetTextureStageState(0, D3DTSS_CONSTANT, dwCONSTANT);
    pD3DHook->pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG0, dwARG0);
    pD3DHook->pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwARG1);
    pD3DHook->pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, dwARG2);
}

void CVisuals::ESP_Distance(int iPlayerID, CPed *pPed) {
    CVector vecFoot = pPed->GetPosition(), vecFootScreen;
    vecFoot.z -= 1.2f;

    Utils::CalcScreenCoors(&vecFoot, &vecFootScreen);
    if (vecFootScreen.z < 1.f)
        return;

    char szDistance[32];
    sprintf(szDistance, "Distance: %.2f m", Utils::getDistance(pPed->GetPosition()));
    CRender::DrawString(pRender->pESPFont, szDistance, vecFootScreen, 14.f, 0xFFFFFFFF, true);
}
