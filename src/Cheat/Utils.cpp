#include <CCamera.h>
#include <common.h>
#include <d3dx9math.h>
#include <Cheat/Utils.h>
#include <CAnimManager.h>
#include <CStreaming.h>
#include <CTaskSimpleRunNamedAnim.h>

void Utils::CalcScreenCoors(const CVector *vecWorld, CVector *vecScreen) {
    const D3DXMATRIX m(reinterpret_cast<float *>(0xB6FA2C));

    const auto dwLenX = reinterpret_cast<DWORD *>(0xC17044);
    const auto dwLenY = reinterpret_cast<DWORD *>(0xC17048);

    vecScreen->x = (vecWorld->z * m._31) + (vecWorld->y * m._21) + (vecWorld->x * m._11) + m._41;
    vecScreen->y = (vecWorld->z * m._32) + (vecWorld->y * m._22) + (vecWorld->x * m._12) + m._42;
    vecScreen->z = (vecWorld->z * m._33) + (vecWorld->y * m._23) + (vecWorld->x * m._13) + m._43;

    const double fRecip = 1.0 / vecScreen->z;
    vecScreen->x *= static_cast<float>(fRecip * (*dwLenX));
    vecScreen->y *= static_cast<float>(fRecip * (*dwLenY));
}

void Utils::CalcWorldCoors(CVector *vecScreen, CVector *vecWorld) {
    D3DXMATRIXA16 m(reinterpret_cast<float *>(0xB6FA2C));

    D3DXMATRIXA16 minv;
    memset(&minv, 0, sizeof(D3DXMATRIXA16));
    m._44 = 1.0f;

    D3DXMatrixInverse(&minv, nullptr, &m);

    const auto *dwLenX = reinterpret_cast<DWORD *>(0xC17044);
    const auto *dwLenY = reinterpret_cast<DWORD *>(0xC17048);

    const float fRecip = 1.0f / vecScreen->z;
    vecScreen->x /= fRecip * (*dwLenX);
    vecScreen->y /= fRecip * (*dwLenY);

    vecWorld->x = vecScreen->z * minv._31 + vecScreen->y * minv._21 + vecScreen->x * minv._11 + minv._41;
    vecWorld->y = vecScreen->z * minv._32 + vecScreen->y * minv._22 + vecScreen->x * minv._12 + minv._42;
    vecWorld->z = vecScreen->z * minv._33 + vecScreen->y * minv._23 + vecScreen->x * minv._13 + minv._43;
}

int Utils::isGTAMenuActive() {
    return *reinterpret_cast<uint8_t *>(0x0BA67A4);
}

float Utils::getDistance(const CVector &vecTarget) {
    const CVector vecSelf = FindPlayerPed()->GetPosition();
    return sqrt(
        (vecSelf.x - vecTarget.x) * (vecSelf.x - vecTarget.x) + (vecSelf.y - vecTarget.y) * (vecSelf.y - vecTarget.y) +
        (vecSelf.z - vecTarget.z) * (vecSelf.z - vecTarget.z));
}

float Utils::getDistanceFromCamera(const CVector &vecTarget) {
    const CVector vecSrc = TheCamera.m_vecGameCamPos;
    return sqrt(
        (vecSrc.x - vecTarget.x) * (vecSrc.x - vecTarget.x) + (vecSrc.y - vecTarget.y) * (vecSrc.y - vecTarget.y) + (
            vecSrc.z - vecTarget.z) * (vecSrc.z - vecTarget.z));
}

void Utils::DisembarkInstantly() {
    CTaskManager *taskManager = &FindPlayerPed()->m_pIntelligence->m_TaskMgr;

    for (int i = 0; i < TASK_PRIMARY_MAX; i++) {
        if (CTask *pTask = taskManager->m_aPrimaryTasks[i]) {
            pTask->MakeAbortable(FindPlayerPed(), ABORT_PRIORITY_IMMEDIATE, nullptr);
            if (i != TASK_PRIMARY_DEFAULT)
                taskManager->SetTask(nullptr, i, false);
        }
    }

    for (int i = 0; i < TASK_SECONDARY_MAX; i++) {
        if (CTask *pTask = taskManager->m_aSecondaryTasks[i]) {
            pTask->MakeAbortable(FindPlayerPed(), ABORT_PRIORITY_IMMEDIATE, nullptr);
            if (i != TASK_SECONDARY_FACIAL_COMPLEX)
                taskManager->SetTaskSecondary(nullptr, i);
        }
    }
}

bool Utils::isPlayingAnimation(CEntity *pEntity, const char *szAnimName) {
    DWORD dwReturn = 0;
    DWORD dwFunc = 0x4D6870;
    auto dwThis = reinterpret_cast<DWORD>(pEntity->m_pRwObject);

    _asm
            {
            push szAnimName
            push dwThis
            call dwFunc
            add esp, 8
            mov dwReturn, eax
            }

    if (dwReturn) return true;
    else return false;
}

CVector *Utils::getBonePosition(CPed *pPed, ePedBones bone, CVector *vecPosition) {
    DWORD dwFunc = 0x5E4280;
    auto dwThis = reinterpret_cast<DWORD>(pPed);

    _asm
            {
            push 1
            push bone
            push vecPosition
            mov ecx, dwThis
            call dwFunc
            }

    return vecPosition;
}

float Utils::vecLength(const CVector &vecPos) {
    return sqrt((vecPos.x * vecPos.x) + (vecPos.y * vecPos.y) + (vecPos.z * vecPos.z));
}
