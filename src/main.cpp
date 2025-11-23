#include "main.h"

#include <rakhook/rakhook.hpp>
#include <CVector.h>
#include <common.h>
#include <D3DX9math.h>
#include <RakNet/PacketEnumerations.h>

#include "Cheat/Config/Config.h"
#include "Features/Visuals.h"
#include "Features/Aimbot.h"
#include "D3D/D3DHook.h"
#include "sampapi/0.3.7-R3-1/CNetGame.h"
#include "SAMP_disabled/SAMP.h"
HMODULE g_hModule = nullptr;


int getNearestPlayer(const bool bTeamProtect) {
    float fNearestDistance = -1.0f;
    int iPlayerID = -1;

    auto *const p_player_pool = sampapi::v037r3::RefNetGame()->GetPlayerPool();
    const auto local_player_color = p_player_pool->GetLocalPlayer()->GetColorAsRGBA();

    for (int i = 0; i < SAMP_MAX_PLAYERS; i++) {
        auto *const p_remote_player = p_player_pool->GetPlayer(i);
        if (p_remote_player == nullptr)
            continue;
        if (p_remote_player->m_pPed == nullptr)
            continue;
        if (p_remote_player->m_nStatus == sampapi::v037r3::CRemotePlayer::PlayerStatus::PLAYER_STATUS_TIMEOUT)
            continue;
        if (!p_remote_player->m_pPed->m_pGamePed->IsAlive())
            continue;
        if (bTeamProtect && p_remote_player->GetColorAsRGBA() == local_player_color)
            continue;

        if (const float fDistance = p_remote_player->GetDistanceToLocalPlayer(); fNearestDistance == -1.0f || fDistance < fNearestDistance) {
            iPlayerID = i;
            fNearestDistance = fDistance;
        }
    }
    return iPlayerID;
}

void initialize_rakhook() {
    rakhook::on_send_packet += [](RakNet::BitStream *bitStream,
                                  PacketPriority &priority,
                                  PacketReliability &reliability,
                                  char &ord_channel) {
        BYTE packetId;
        bitStream->Read(packetId);

        if (g_Config.g_Player.bFakeLag) {
            static ULONGLONG dwSyncTick = GetTickCount64();
            if (GetTickCount64() - dwSyncTick > static_cast<ULONGLONG>(g_Config.g_Player.iFakeLag))
                dwSyncTick = GetTickCount64();
        }

        switch (packetId) {
            case ID_PLAYER_SYNC: {
                if (g_Config.g_Developer.bDamager) {
                    sampapi::v037r3::Synchronization::BulletData BulletData;
                    ZeroMemory(&BulletData, sizeof(sampapi::v037r3::Synchronization::BulletData));
                    int iNearest = getNearestPlayer(g_Config.g_Developer.bTeamProtect);

                    auto *const p_player_pool = sampapi::v037r3::RefNetGame()->GetPlayerPool();
                    auto *const p_local_player = p_player_pool->GetLocalPlayer();
                    auto *const p_remote_player = p_player_pool->GetPlayer(iNearest);

                    static ULONGLONG ulTick = 0;
                    if ((!g_Config.g_Developer.bDelay || (GetTickCount64() - ulTick) >= g_Config.g_Developer.iDelay)) {
                        int iWeaponID = g_Config.g_Developer.bCustomWeapon
                                            ? g_Config.g_Developer.iWeaponID
                                            : p_local_player->m_weaponsData.m_nCurrentWeapon;
                        float fDamage = g_Config.g_Developer.bCustomDamage
                                            ? g_Config.g_Developer.fDamage
                                            : fWeaponDamage[p_local_player->m_weaponsData.m_nCurrentWeapon];

                        if (g_Config.g_Developer.bTeleportToPlayer) {
                            CVector vecPos = p_remote_player->m_pPed->m_pGamePed->GetPosition();
                            vecPos.x += 1.5f;
                            FindPlayerPed()->SetPosn(vecPos);
                        }

                        if (g_Config.g_Developer.bSendBulletData) {
                            BulletData.m_nTargetType = 1;
                            BulletData.m_nTargetId = static_cast<uint16_t>(iNearest);
                            BulletData.m_nWeapon = static_cast<uint8_t>(iWeaponID);

                            const CVector &local_ped_position = p_local_player->m_pPed->m_pGamePed->GetPosition();
                            BulletData.m_origin = sampapi::CVector(local_ped_position.x,
                                                                   local_ped_position.y,
                                                                   local_ped_position.z);

                            const CVector &remote_ped_position = p_remote_player->m_pPed->m_pGamePed->GetPosition();
                            BulletData.m_target = sampapi::CVector(remote_ped_position.x,
                                                                   remote_ped_position.y,
                                                                   remote_ped_position.z);

                            if (p_remote_player->m_nState ==
                                PLAYER_STATE_ONFOOT) {
                                BulletData.m_center.x = (1.f / 2.f) - ((rand() % static_cast<int>(1.f * 10.0f) / 100.0f));
                                BulletData.m_center.y = (1.f / 2.f) - ((rand() % static_cast<int>(1.f * 10.0f) / 100.0f));
                                BulletData.m_center.z = 0.50f - ((rand() % 100) / 100.0f);
                            } else {
                                BulletData.m_center.x = 0.0f;
                                BulletData.m_center.y = 0.0f;
                                BulletData.m_center.z = 0.56f - ((rand() % 5) / 100.0f);
                            }

                            RakNet::BitStream bsBulletData;
                            bsBulletData.Write(static_cast<BYTE>(ID_BULLET_SYNC));
                            bsBulletData.Write(reinterpret_cast<PCHAR>(&BulletData),
                                               sizeof(sampapi::v037r3::Synchronization::BulletData));
                            rakhook::send(&bsBulletData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
                        }

                        RakNet::BitStream bsGiveDamage;
                        bsGiveDamage.Write(false);
                        bsGiveDamage.Write(static_cast<USHORT>(iNearest));
                        bsGiveDamage.Write(fDamage);
                        bsGiveDamage.Write((int) iWeaponID);
                        bsGiveDamage.Write((rand() % 7) + 3);
                        rakhook::send_rpc(115, &bsGiveDamage, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false);
                        ulTick = GetTickCount64();
                    }
                }
                if (g_Config.g_Player.bUpsideMode || g_Config.g_Player.bInvertWalk || g_Config.g_Player.bInvisible) {
                    bool bEditFootSync = false;
                    sampapi::v037r3::Synchronization::OnfootData OnFootData;
                    bitStream->ResetReadPointer();
                    bitStream->Read(packetId);
                    bitStream->Read(reinterpret_cast<char *>(&OnFootData), sizeof(sampapi::v037r3::Synchronization::OnfootData));

                    if (g_Config.g_Player.bUpsideMode) {
                        bEditFootSync = true;
                        D3DXQUATERNION Quat(OnFootData.m_fQuaternion[1],
                                            OnFootData.m_fQuaternion[2],
                                            OnFootData.m_fQuaternion[3],
                                            OnFootData.m_fQuaternion[0]), Quat2;
                        D3DXVECTOR3 Axis(0, 1, 0);
                        D3DXQuaternionRotationAxis(&Quat2, &Axis, M_PI);
                        D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
                        OnFootData.m_fQuaternion[0] = Quat.w;
                        OnFootData.m_fQuaternion[1] = Quat.x;
                        OnFootData.m_fQuaternion[2] = Quat.y;
                        OnFootData.m_fQuaternion[3] = Quat.z;
                    }

                    if (g_Config.g_Player.bInvertWalk) {
                        bEditFootSync = true;
                        D3DXQUATERNION Quat(OnFootData.m_fQuaternion[1],
                                            OnFootData.m_fQuaternion[2],
                                            OnFootData.m_fQuaternion[3],
                                            OnFootData.m_fQuaternion[0]), Quat2;
                        D3DXVECTOR3 Axis(0, 0, 1);
                        D3DXQuaternionRotationAxis(&Quat2, &Axis, M_PI);
                        D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
                        OnFootData.m_fQuaternion[0] = Quat.w;
                        OnFootData.m_fQuaternion[1] = Quat.x;
                        OnFootData.m_fQuaternion[2] = Quat.y;
                        OnFootData.m_fQuaternion[3] = Quat.z;
                    }

                    if (bEditFootSync) {
                        bitStream->Reset();
                        bitStream->Write(static_cast<BYTE>(ID_PLAYER_SYNC));
                        bitStream->Write(reinterpret_cast<PCHAR>(&OnFootData), sizeof(sampapi::v037r3::Synchronization::OnfootData));
                    }
                }
                break;
            }
            case ID_VEHICLE_SYNC: {
                bool bEditVehicleSync = false;
                sampapi::v037r3::Synchronization::IncarData InCar;
                memcpy(&InCar,
                       &sampapi::v037r3::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->m_incarData,
                       sizeof(sampapi::v037r3::Synchronization::IncarData));

                if (g_Config.g_Player.bUpsideMode) {
                    bEditVehicleSync = true;
                    D3DXQUATERNION Quat(InCar.m_fQuaternion[1],
                                        InCar.m_fQuaternion[2],
                                        InCar.m_fQuaternion[3],
                                        InCar.m_fQuaternion[0]), Quat2;
                    D3DXVECTOR3 Axis(0, 1, 0);
                    D3DXQuaternionRotationAxis(&Quat2, &Axis, M_PI);
                    D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
                    InCar.m_fQuaternion[0] = Quat.w;
                    InCar.m_fQuaternion[1] = Quat.x;
                    InCar.m_fQuaternion[2] = Quat.y;
                    InCar.m_fQuaternion[3] = Quat.z;
                }

                if (g_Config.g_Player.bInvertWalk) {
                    bEditVehicleSync = true;
                    D3DXQUATERNION Quat(InCar.m_fQuaternion[1],
                                        InCar.m_fQuaternion[2],
                                        InCar.m_fQuaternion[3],
                                        InCar.m_fQuaternion[0]), Quat2;
                    D3DXVECTOR3 Axis(0, 0, 1);
                    D3DXQuaternionRotationAxis(&Quat2, &Axis, M_PI);
                    D3DXQuaternionMultiply(&Quat, &Quat, &Quat2);
                    InCar.m_fQuaternion[0] = Quat.w;
                    InCar.m_fQuaternion[1] = Quat.x;
                    InCar.m_fQuaternion[2] = Quat.y;
                    InCar.m_fQuaternion[3] = Quat.z;
                }

                if (bEditVehicleSync) {
                    bitStream->Reset();
                    bitStream->Write(static_cast<BYTE>(ID_VEHICLE_SYNC));
                    bitStream->Write(reinterpret_cast<PCHAR>(&InCar), sizeof(sampapi::v037r3::Synchronization::IncarData));
                }
                break;
            }
            case ID_BULLET_SYNC: {
                if (g_Config.g_Visuals.bDMGInformer) {
                    sampapi::v037r3::Synchronization::BulletData BulletData;
                    bitStream->ResetReadPointer();
                    bitStream->Read(packetId);
                    bitStream->Read(reinterpret_cast<PCHAR>(&BulletData), sizeof(sampapi::v037r3::Synchronization::BulletData));

                    if (BulletData.m_nTargetType == 1) {
                        //pVisuals->fHitmarkerAlpha = 1.f;

                        CVector vecTarget(BulletData.m_target.x, BulletData.m_target.y, BulletData.m_target.z);

                        int iBefore = 0;
                        for (int i = 1; i < _countof(pVisuals->g_DMGInformer); i++)
                            if (pVisuals->g_DMGInformer[i].dwTick < pVisuals->g_DMGInformer[iBefore].dwTick)
                                iBefore = i;

                        pVisuals->g_DMGInformer[iBefore].iWeapon = sampapi::v037r3::RefNetGame()->GetPlayerPool()->
                                GetLocalPlayer()->m_weaponsData.m_nCurrentWeapon;
                        pVisuals->g_DMGInformer[iBefore].vecPos = vecTarget;
                        pVisuals->g_DMGInformer[iBefore].dwTick =
                                GetTickCount64() + static_cast<ULONGLONG>(g_Config.g_Visuals.fDMGTick
                                                                          * 1000.f);
                    }
                }
                break;
            }
            case ID_AIM_SYNC: {
                if (g_Config.g_Player.bShakeMode || g_Config.g_Player.bInvalidAimZ || g_Config.g_Visuals.bAspectRatio) {
                    bool bEditAimSync = false;
                    sampapi::v037r3::Synchronization::AimData AimData;
                    bitStream->ResetReadPointer();
                    bitStream->Read(packetId);
                    bitStream->Read(reinterpret_cast<PCHAR>(&AimData), sizeof(sampapi::v037r3::Synchronization::AimData));
                    if (g_Config.g_Visuals.bAspectRatio) {
                        bEditAimSync = true;
                        AimData.m_nAspectRatio = 0x55;
                    }
                    if (g_Config.g_Player.bInvalidAimZ) {
                        bEditAimSync = true;
                        AimData.m_fAimZ = NAN;
                    }
                    if (g_Config.g_Player.bShakeMode) {
                        bEditAimSync = true;
                        BYTE byteCam[2] = {34, 45};
                        AimData.m_nCameraMode = byteCam[rand() % 2];
                        AimData.m_nWeaponState = 2;
                    }
                    if (bEditAimSync) {
                        bitStream->Reset();
                        bitStream->Write(static_cast<BYTE>(ID_AIM_SYNC));
                        bitStream->Write(reinterpret_cast<PCHAR>(&AimData), sizeof(sampapi::v037r3::Synchronization::AimData));
                    }
                }
                break;
            }
            default: break;
        }
        return true;
    };
}

void
mainThread() {
    pConfig = new CConfig("Stealth");

    while (!rakhook::initialize())
        Sleep(100);

    initialize_rakhook();

    pD3DHook = new CD3DHook();
    pAimbot = new CAimbot();
    pVisuals = new CVisuals();

    ExitThread(0);
}

BOOL APIENTRY DllMain(const HMODULE hModule,
                      const DWORD dwReasonForCall,
                      LPVOID lpReserved) {
    if (dwReasonForCall == DLL_PROCESS_ATTACH) {
        g_hModule = hModule;
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(mainThread), nullptr, NULL, nullptr);
    }
    return TRUE;
}
