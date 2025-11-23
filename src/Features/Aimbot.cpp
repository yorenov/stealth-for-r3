#include "Aimbot.h"

#include <CCamera.h>
#include <common.h>
#include <CWorld.h>
#include <ePedBones.h>
#include <imgui.h>
#include <CRunningScript.h>

#include "cheat/KeyHook.h"
#include "cheat/Structs.h"
#include "Cheat/Config/Config.h"
#include "imgui/imgui_render.h"
#include "sampapi/0.3.7-R3-1/CNetGame.h"
#include "SAMP_disabled/SAMP.h"
#include "Cheat/Utils.h"
#include "cheat/MathStuff.h"
#include "cheat/Secure.h"
CAimbot *pAimbot;

void
CAimbot::Update() {
    const int iMode = TheCamera.m_aCams[0].m_nMode;
    bCrosshair = iMode == 53 || iMode == 7;

    if (sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->m_weaponsData.m_nCurrentWeapon != 34) {
        vecCrosshair.x = ImGui::GetIO().DisplaySize.x * 0.5299999714f;
        vecCrosshair.y = ImGui::GetIO().DisplaySize.y * 0.4f;
    } else {
        vecCrosshair.x = ImGui::GetIO().DisplaySize.x / 2.f;
        vecCrosshair.y = ImGui::GetIO().DisplaySize.y / 2.f;
    }

    GetAimingPlayer();
    SmoothAimbot();
    ProAimbot();
    Triggerbot();
}

void
CAimbot::Render() const {
    const auto local_player = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetLocalPlayer();

    if (const sampapi::NUMBER m_n_current_weapon = local_player->m_weaponsData.m_nCurrentWeapon;
        bCrosshair && g_Config.g_Aimbot.bAimbot && g_Config.g_Aimbot.bAimbotEnabled[m_n_current_weapon]) {
        if (g_Config.g_Aimbot.bDrawRange)
            g_Config.g_Aimbot.iRangeStyle
                ? CRender::DrawCircle(vecCrosshair,
                                      static_cast<float>(g_Config.g_Aimbot.iAimbotConfig[m_n_current_weapon][RANGE]),
                                      g_Config.g_Aimbot.colorRange,
                                      g_Config.g_Aimbot.fOutlineThickness)
                : CRender::DrawCircleFilled(vecCrosshair,
                                            static_cast<float>(g_Config.g_Aimbot.iAimbotConfig[m_n_current_weapon][
                                                RANGE]),
                                            g_Config.g_Aimbot.colorRange);

        if (g_Config.g_Aimbot.bDrawTracer && iTargetPlayer != -1)
            CRender::DrawLine(vecCrosshair, vecTargetBone, ImColor(0.f, 1.f, 0.f), g_Config.g_Aimbot.fOutlineThickness);
    }
}

void
CAimbot::GetAimingPlayer() {
    iTargetPlayer = -1;

    const auto player_pool = sampapi::v037r3::RefNetGame()->GetPlayerPool();

    if (g_Config.g_Aimbot.bAimbot || g_Config.g_Aimbot.bSmooth || g_Config.g_Aimbot.bSilent || g_Config.g_Aimbot.
        bProAim) {
        float fNearestDistance = 9999.f;
        CVector vecCamera, vecOrigin, vecTarget;
        Utils::getBonePosition(FindPlayerPed(), BONE_RIGHTWRIST, &vecOrigin);
        TheCamera.Find3rdPersonCamTargetVector(100.f, vecOrigin, &vecCamera, &vecTarget);

        const auto local_player = player_pool->GetLocalPlayer();
        for (int i = 0; i < SAMP_MAX_PLAYERS; i++) {
            const auto remote_player = player_pool->GetPlayer(i);
            if (remote_player == nullptr || !remote_player->DoesExist())
                continue;

            CPed *pPed = remote_player->m_pPed->m_pGamePed;
            if (!pPed || !pPed->IsAlive())
                continue;

            if (g_Config.g_Aimbot.bTeamProtect && remote_player->GetColorAsRGBA() == local_player->GetColorAsRGBA())
                continue;

            if (!g_Config.g_Aimbot.bIgnoreEverything) {
                if (g_Config.g_Aimbot.bIgnoreAFK && remote_player->m_nStatus ==
                    sampapi::v037r3::CRemotePlayer::PLAYER_STATUS_TIMEOUT)
                    continue;

                if (const float target_distance = remote_player->m_pPed->GetDistanceToLocalPlayer();
                    !g_Config.g_Aimbot.bIgnoreMaxDistance && target_distance > fWeaponRange[local_player->m_weaponsData.
                        m_nCurrentWeapon])
                    continue;
            }

            for (auto iBone: iBoneList) {
                CVector vecBone;
                CVector vecBoneScreen;
                Utils::getBonePosition(pPed, static_cast<ePedBones>(iBone), &vecBone);
                Utils::CalcScreenCoors(&vecBone, &vecBoneScreen);
                if (vecBoneScreen.z < 1.0f)
                    continue;

                const float fCentreDistance = Math::vect2_dist(&vecCrosshair, &vecBoneScreen);
                if (g_Config.g_Aimbot.bAimbot && fCentreDistance >= static_cast<float>(g_Config.g_Aimbot.iAimbotConfig[
                        local_player
                        ->m_weaponsData.m_nCurrentWeapon][RANGE]) * 1.5f)
                    continue;

                if (!g_Config.g_Aimbot.bIgnoreEverything && !g_Config.g_Aimbot.bLockThroughObjects && !
                    CWorld::GetIsLineOfSightClear(vecCamera, vecBone, true, true, false, true, true, true, false))
                    continue;

                if (fCentreDistance <= fNearestDistance) {
                    fNearestDistance = fCentreDistance;
                    iTargetPlayer = i;
                    iTargetBone = iBone;
                    vecTargetBone = vecBoneScreen;
                }
            }
        }
    }
}

bool
__stdcall CAimbot::hkFireInstantHit(void *this_,
                                    CEntity *pFiringEntity,
                                    CVector *pOrigin,
                                    CVector *pMuzzle,
                                    CEntity *pTargetEntity,
                                    CVector *pTarget,
                                    CVector *pVec,
                                    const bool bCrossHairGun,
                                    const bool bCreateGunFx) {
    if (const auto local_player = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetLocalPlayer();
        pFiringEntity == static_cast<CEntity *>(FindPlayerPed()) && g_Config.g_Aimbot.bSilent && pAimbot->iTargetPlayer
        != -1 &&
        g_Config.g_Aimbot.bAimbotEnabled[local_player->m_weaponsData.m_nCurrentWeapon] && rand() % 100 <=
        g_Config.g_Aimbot.iAimbotConfig[local_player->m_weaponsData.m_nCurrentWeapon][SILENT]) {
        if (auto *pPed = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetPlayer(pAimbot->iTargetPlayer)->m_pPed->
                    m_pGamePed;
            pPed != nullptr) {
            CVector vecCamera;
            CVector vecOrigin;
            CVector vecTarget;

            Utils::getBonePosition(FindPlayerPed(), BONE_RIGHTWRIST, &vecOrigin);
            TheCamera.Find3rdPersonCamTargetVector(100.F, vecOrigin, &vecCamera, &vecTarget);
            if (pAimbot->bCrosshair)
                pOrigin = &vecCamera;

            CVector vecBone;
            Utils::getBonePosition(pPed, static_cast<ePedBones>(pAimbot->iTargetBone), &vecBone);
            pTarget = &vecBone;

            if (g_Config.g_Aimbot.bLockThroughObjects || g_Config.g_Aimbot.bIgnoreEverything)
                pSecure->memcpy_safe(reinterpret_cast<void *>(0x740B4E),
                                     "\x6A\x00\x6A\x00",
                                     4);
            else
                pSecure->memcpy_safe(reinterpret_cast<void *>(0x740B4E),
                                     "\x6A\x01\x6A\x01",
                                     4);
            pSecure->Write(0x8D6114, g_Config.g_Aimbot.fSilentAccuracy / 20.f);
        }
    } else {
        Memory::memcpy_safe(reinterpret_cast<void *>(0x740B4E), "\x6A\x01\x6A\x01", 4);
        *reinterpret_cast<float *>(0x8D6114) = 5.F;
    }
    return pAimbot->oFireInstantHit(this_,
                                    pFiringEntity,
                                    pOrigin,
                                    pMuzzle,
                                    pTargetEntity,
                                    pTarget,
                                    pVec,
                                    bCrossHairGun,
                                    bCreateGunFx);
}

bool
__cdecl CAimbot::hkAddBullet(CEntity *pCreator,
                             const eWeaponType weaponType,
                             const CVector &vecPosition,
                             CVector vecVelocity) {
    if (pCreator == static_cast<CEntity *>(FindPlayerPed()) && g_Config.g_Aimbot.bSilent && pAimbot->iTargetPlayer != -1
        && g_Config.
        g_Aimbot.bAimbotEnabled[34] && rand() % 100 <= g_Config.g_Aimbot.iAimbotConfig[34][SILENT]) {
        if (auto *pPed = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetPlayer(pAimbot->iTargetPlayer)->m_pPed->
                    m_pGamePed;
            pPed != nullptr) {
            CVector vecBone;
            Utils::getBonePosition(pPed, static_cast<ePedBones>(pAimbot->iTargetBone), &vecBone);
            vecVelocity = vecBone - vecPosition;

            if (g_Config.g_Aimbot.bLockThroughObjects || g_Config.g_Aimbot.bIgnoreEverything)
                pSecure->memcpy_safe(reinterpret_cast<void *>(0x736212),
                                     "\x6A\x00\x6A\x00",
                                     4);
            else
                pSecure->memcpy_safe(reinterpret_cast<void *>(0x736212),
                                     "\x6A\x01\x6A\x01",
                                     4);
        }
    } else Memory::memcpy_safe(reinterpret_cast<void *>(0x736212), "\x6A\x01\x6A\x01", 4);

    return pAimbot->oAddBullet(pCreator, weaponType, vecPosition, vecVelocity);
}

float
__cdecl CAimbot::hkTargetWeaponRangeMultiplier(CEntity *pVictim,
                                               CEntity *pOwner) {
    if (pOwner == static_cast<CEntity *>(FindPlayerPed()) && (g_Config.g_Aimbot.bIgnoreMaxDistance || g_Config.g_Aimbot.
                                                              bIgnoreEverything))
        return 100.f;

    return pAimbot->oTargetWeaponRangeMultiplier(pVictim, pOwner);
}

void
CAimbot::SmoothAimbot() const {
    if (const sampapi::NUMBER m_n_current_weapon = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->
            m_weaponsData
            .m_nCurrentWeapon; g_Config.g_Aimbot.bSmooth && g_Config.g_Aimbot.bAimbotEnabled[m_n_current_weapon] &&
                               iTargetPlayer != -1 &&
                               bCrosshair) {
        if (g_Config.g_Aimbot.bSmoothIsFire && !GetAsyncKeyState(VK_LBUTTON))
            return;

        auto *pPed = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetPlayer(pAimbot->iTargetPlayer)->m_pPed->
                m_pGamePed;
        if (pPed == nullptr)
            return;

        if (g_Config.g_Aimbot.bStopOnBody) {
            CVector vecCamera, vecOrigin, vecTarget;
            if (m_n_current_weapon != 34) {
                Utils::getBonePosition(FindPlayerPed(), BONE_RIGHTWRIST, &vecOrigin);
                TheCamera.Find3rdPersonCamTargetVector(100.f, vecOrigin, &vecCamera, &vecTarget);
            } else {
                CVector vecFront = TheCamera.m_aCams[0].m_vecFront;
                vecFront.Normalise();
                vecOrigin = *TheCamera.GetGameCamPosition();
                vecOrigin += vecFront * 2.0f;
                vecTarget = vecOrigin + vecFront * 100.f;
            }
            CColPoint pCollision;
            CEntity *pCollisionEntity = nullptr;
            if (const bool bCollision = CWorld::ProcessLineOfSight(vecCamera,
                                                                   vecTarget,
                                                                   pCollision,
                                                                   pCollisionEntity,
                                                                   !g_Config.g_Aimbot.bLockThroughObjects,
                                                                   !g_Config.g_Aimbot.bLockThroughObjects,
                                                                   true,
                                                                   true,
                                                                   true,
                                                                   true,
                                                                   false,
                                                                   true); bCollision && pCollisionEntity &&
                                                                          pCollisionEntity->m_nType == ENTITY_TYPE_PED
                                                                          &&
                                                                          pCollisionEntity == static_cast<CEntity *>(
                                                                              pPed))
                return;
        }

        const CVector vecOrigin = *TheCamera.GetGameCamPosition();
        CVector vecTarget = *Utils::getBonePosition(
            pPed,
            BONE_PELVIS1,
            &vecTarget);
        const CVector vecVector = vecOrigin - vecTarget;

        const auto *fAspectRatio = reinterpret_cast<float *>(0xC3EFA4);
        const auto *fCrosshairOffset = reinterpret_cast<float *>(0xB6EC10);

        float fFix = 0.f;
        float fVecX = 0.f;
        float z = 0.f;
        float x = 0.f;

        if (TheCamera.m_aCams[0].m_nMode == 53 || TheCamera.m_aCams[0].m_nMode == 55) {
            const float fMult = tan(TheCamera.FindCamFOV() / 2.0f * 0.017453292f);
            z = M_PI - atan2(1.0f, fMult * ((1.0f - fCrosshairOffset[0] * 2.0f) * (1.0f / *fAspectRatio)));
            x = M_PI - atan2(1.0f, fMult * (fCrosshairOffset[1] * 2.0f - 1.0f));
        } else x = z = M_PI / 2;

        const BYTE byteWeapon = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->m_weaponsData.
                m_nCurrentWeapon;
        if (byteWeapon >= 22 && byteWeapon <= 29 || byteWeapon == 32) fFix = 0.04253f;
        else if (byteWeapon == 30 || byteWeapon == 31) fFix = 0.028f;
        else if (byteWeapon == 33) fFix = 0.01897f;

        const float fDistX = sqrt(vecVector.x * vecVector.x + vecVector.y * vecVector.y);

        if (vecVector.x <= 0.0 && vecVector.y >= 0.0 || vecVector.x >= 0.0 && vecVector.y >= 0.0)
            fVecX = acosf(vecVector.x / fDistX) + fFix - TheCamera.m_aCams[0].m_fHorizontalAngle;
        if (vecVector.x >= 0.0 && vecVector.y <= 0.0 || vecVector.x <= 0.0 && vecVector.y <= 0.0)
            fVecX = -acosf(vecVector.x / fDistX) + fFix - TheCamera.m_aCams[0].m_fHorizontalAngle;

        if (const float fSmoothX = fVecX / (static_cast<float>(g_Config.g_Aimbot.iAimbotConfig[byteWeapon][SMOOTH]) * 2)
            ; fSmoothX > -1.0 && fSmoothX < 0.5 && fVecX > -2.0 && fVecX < 2.0)
            TheCamera.m_aCams[0].m_fHorizontalAngle += fSmoothX;

        if (g_Config.g_Aimbot.bSmoothLockY) {
            const float fDistZ = sqrt(vecVector.x * vecVector.x + vecVector.y * vecVector.y);
            const float fSmoothZ = (atan2f(fDistZ, vecVector.z) - z - TheCamera.m_aCams[0].m_fVerticalAngle) / (
                                       static_cast<float>(g_Config.g_Aimbot.iAimbotConfig[byteWeapon][SMOOTH]) * 2);
            TheCamera.m_aCams[0].m_fVerticalAngle += fSmoothZ;
        }
    }
}

void
CAimbot::ProAimbot() const {
    if (const sampapi::NUMBER m_n_current_weapon = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->
                m_weaponsData.m_nCurrentWeapon;
        g_Config.g_Aimbot.bProAim && g_Config.g_Aimbot.bAimbotEnabled[m_n_current_weapon] && iTargetPlayer != -1 &&
        bCrosshair && GetAsyncKeyState(VK_LBUTTON)) {
        auto *pPed = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetPlayer(pAimbot->iTargetPlayer)->m_pPed->
                m_pGamePed;
        if (pPed == nullptr)
            return;

        CVector vecCamera;
        CVector vecOrigin;
        CVector vecTarget;

        if (m_n_current_weapon != 34) {
            Utils::getBonePosition(FindPlayerPed(), BONE_RIGHTWRIST, &vecOrigin);
            TheCamera.Find3rdPersonCamTargetVector(2.f, vecOrigin, &vecCamera, &vecTarget);
        } else {
            CVector vecFront = TheCamera.m_aCams[0].m_vecFront;
            vecFront.Normalise();
            vecOrigin = *TheCamera.GetGameCamPosition();
            vecOrigin += vecFront * 2.0f;
            vecTarget = vecOrigin + vecFront * 1.5f;
        }

        pPed->SetPosn(vecTarget);
    }
}

void
CAimbot::Triggerbot() const {
    if (const sampapi::NUMBER m_n_current_weapon = sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->
                m_weaponsData.m_nCurrentWeapon;
        g_Config.g_Aimbot.bTriggerbot && g_Config.g_Aimbot.bAimbotEnabled[m_n_current_weapon] && bCrosshair) {
        static ULONGLONG iTick = GetTickCount64();
        if (g_Config.g_Aimbot.bTriggerDelay && GetTickCount64() - iTick < static_cast<ULONGLONG>(g_Config.g_Aimbot.
                iTriggerDelay))
            return;

        if (g_Config.g_Aimbot.bTriggerSync) {
            if (g_Config.g_Aimbot.bSilent && g_Config.g_Aimbot.bAimbot && iTargetPlayer != -1) {
                pKeyHook->g_GameKeyState[BUTTON_CIRCLE] = {0xFF, true};
                iTick = GetTickCount64();
            }
        } else {
            CVector vecCamera, vecOrigin, vecTarget;
            if (m_n_current_weapon != 34) {
                Utils::getBonePosition(FindPlayerPed(), BONE_RIGHTWRIST, &vecOrigin);
                TheCamera.Find3rdPersonCamTargetVector(100.f, vecOrigin, &vecCamera, &vecTarget);
            } else {
                CVector vecFront = TheCamera.m_aCams[0].m_vecFront;
                vecFront.Normalise();
                vecOrigin = *TheCamera.GetGameCamPosition();
                vecOrigin += vecFront * 2.0f;
                vecTarget = vecOrigin + vecFront * 100.f;
            }

            CColPoint pCollision;
            CEntity *pCollisionEntity = nullptr;

            if (const bool bCollision = CWorld::ProcessLineOfSight(vecCamera,
                                                                   vecTarget,
                                                                   pCollision,
                                                                   pCollisionEntity,
                                                                   !g_Config.g_Aimbot.bLockThroughObjects,
                                                                   !g_Config.g_Aimbot.bLockThroughObjects,
                                                                   true,
                                                                   true,
                                                                   true,
                                                                   true,
                                                                   false,
                                                                   true); bCollision && pCollisionEntity != nullptr &&
                                                                          pCollisionEntity->m_nType ==
                                                                          ENTITY_TYPE_PED) {
                auto *player_pool = sampapi::v037r3::RefNetGame()->GetPlayerPool();
                auto *local_player = player_pool->GetLocalPlayer();
                for (int i = 0; i < SAMP_MAX_PLAYERS; i++) {
                    auto *const remote_player = player_pool->GetPlayer(i);
                    if (remote_player == nullptr || !remote_player->DoesExist())
                        continue;

                    auto *pPed = remote_player->m_pPed->m_pGamePed;
                    if (pPed == nullptr || pPed == FindPlayerPed())
                        continue;

                    if (pCollisionEntity != static_cast<CEntity *>(pPed))
                        continue;

                    if (pPed->m_fHealth <= 0.f)
                        continue;

                    if (g_Config.g_Aimbot.bTeamProtect && remote_player->GetColorAsRGBA() == local_player->
                        GetColorAsRGBA())
                        continue;

                    if (!g_Config.g_Aimbot.bIgnoreEverything) {
                        if (g_Config.g_Aimbot.bIgnoreAFK && remote_player->m_nStatus ==
                            sampapi::v037r3::CRemotePlayer::PLAYER_STATUS_TIMEOUT)
                            continue;

                        if (const float target_distance = remote_player->m_pPed->GetDistanceToLocalPlayer();
                            !g_Config.g_Aimbot.bIgnoreMaxDistance && target_distance > fWeaponRange[local_player->
                                m_weaponsData.
                                m_nCurrentWeapon])
                            continue;
                    }

                    pKeyHook->g_GameKeyState[BUTTON_CIRCLE] = {0xFF, true};
                    iTick = GetTickCount64();
                    break;
                }
            }
        }
    }
}
