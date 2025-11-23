#pragma once
#include <CEntity.h>
#include <CVector.h>

#include "Cheat/Memory.h"
#include <detours.h>
#include <eWeaponType.h>

#include "cheat/Secure.h"
#include "sampapi/sampapi.h"

class CAimbot {
public:
    CAimbot() {
        iTargetBone = 1;
        iTargetPlayer = -1;
        bCrosshair = false;
        vecTargetBone = {0.f, 0.f, 0.f};
        vecCrosshair = {0.f, 0.f, 0.f};

        oFireInstantHit = reinterpret_cast<tFireInstantHit>(sampapi::GetAddress(0xB5C50));
        oAddBullet = reinterpret_cast<tAddBullet>(sampapi::GetAddress(0xA51E0));
        oTargetWeaponRangeMultiplier = reinterpret_cast<tTargetWeaponRangeMultiplier>(0x73B380);
        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        pSecure->SDetourAttach(&reinterpret_cast<PVOID &>(oFireInstantHit), hkFireInstantHit);
        pSecure->SDetourAttach(&reinterpret_cast<PVOID &>(oAddBullet), hkAddBullet);
        DetourTransactionCommit();
        pSecure->HookInstallCall((DWORD) 0x73FF11, reinterpret_cast<DWORD>(hkTargetWeaponRangeMultiplier));
    };

    ~CAimbot() {
        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&reinterpret_cast<PVOID &>(oFireInstantHit), hkFireInstantHit);
        DetourDetach(&reinterpret_cast<PVOID &>(oAddBullet), hkAddBullet);
        DetourTransactionCommit();
        Memory::memcpy_safe(reinterpret_cast<void *>(0x73FF11), "\xE8\x6A\xB4\xFF\xFF", 5);
    };

    void
    Update();

    void
    Render() const;

    CVector vecCrosshair;

private:
    bool bCrosshair;
    int iTargetPlayer, iTargetBone;
    CVector vecTargetBone;

    void
    GetAimingPlayer();

    void
    SmoothAimbot() const;

    void
    ProAimbot() const;

    void
    Triggerbot() const;

    typedef bool
    (__stdcall*tFireInstantHit)(void *,
                                CEntity *,
                                CVector *,
                                CVector *,
                                CEntity *,
                                CVector *,
                                CVector *,
                                bool,
                                bool);

    typedef bool
    (__cdecl*tAddBullet)(CEntity *,
                         eWeaponType,
                         CVector,
                         CVector);

    typedef float
    (__cdecl*tTargetWeaponRangeMultiplier)(CEntity *,
                                           CEntity *);

    static bool
    __stdcall hkFireInstantHit(void *this_,
                               CEntity *pFiringEntity,
                               CVector *pOrigin,
                               CVector *pMuzzle,
                               CEntity *pTargetEntity,
                               CVector *pTarget,
                               CVector *pVec,
                               bool bCrossHairGun,
                               bool bCreateGunFx);

    static bool
    __cdecl hkAddBullet(CEntity *pCreator,
                        eWeaponType weaponType,
                        const CVector &vecPosition,
                        CVector vecVelocity);

    static float
    __cdecl hkTargetWeaponRangeMultiplier(CEntity *pVictim,
                                          CEntity *pOwner);

    tFireInstantHit oFireInstantHit;
    tAddBullet oAddBullet;
    tTargetWeaponRangeMultiplier oTargetWeaponRangeMultiplier;
};

extern CAimbot *pAimbot;
