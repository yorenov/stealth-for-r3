#include "Features/Player.h"
#include "Cheat/Utils.h"
#include "cheat/KeyHook.h"

#include <common.h>
#include <CWorld.h>

#include "Cheat/Config/Config.h"
CPed* Player::g_p_ped;
bool Player::bExitVehicle = false;
ULONGLONG Player::dwExitTick = 0;

void
Player::Update() {
	g_p_ped = FindPlayerPed();
	if (!g_p_ped->IsAlive())
		return;

	AirBreak();
	AntiStun();
}

void Player::AirBreak() {
	if (g_Config.g_Player.bAirBreak || g_Config.g_Player.bMapRun) {
		const float fCameraRotation = *reinterpret_cast<float *>(0xB6F178);
		const float fSpeed = g_Config.g_Player.bAirBreak
			                     ? static_cast<float>(g_Config.g_Player.iAirBreak) / 10.f
			                     : static_cast<float>(g_Config.g_Player.iMapRun) / 10.f;
		CVector* nVec = &g_p_ped->m_matrix->pos;
		g_p_ped->m_fCurrentRotation = g_p_ped->m_fAimingRotation = -fCameraRotation;
		g_p_ped->m_vecMoveSpeed.z = 0.f;

		if (CVehicle* pVehicle = FindPlayerVehicle(-1, false)) {
			nVec = &pVehicle->m_matrix->pos;
			const float fDiff = Utils::vecLength(pVehicle->m_matrix->pos - *nVec);
			pVehicle->m_matrix->SetRotateZOnly(-fCameraRotation);
			pVehicle->m_matrix->pos.x = nVec->x - sinf(fCameraRotation) * fDiff;
			pVehicle->m_matrix->pos.y = nVec->y - cosf(fCameraRotation) * fDiff;
			pVehicle->m_vecMoveSpeed.x = pVehicle->m_vecMoveSpeed.y = pVehicle->m_vecMoveSpeed.z = 0.f;
		} else if (g_Config.g_Player.bAirBreak)
			g_p_ped->m_nPedFlags.bIsStanding = g_p_ped->m_nPedFlags.bWasStanding = g_p_ped->m_nPedFlags.bStayInSamePlace
				= true;
		else g_p_ped->m_nPedFlags.bIsStanding = true;

		if (isKeyDown('W'))
			nVec->x += sinf(fCameraRotation) * fSpeed, nVec->y += cosf(fCameraRotation) * fSpeed;
		if (isKeyDown('S'))
			nVec->x -= sinf(fCameraRotation) * fSpeed, nVec->y -= cosf(fCameraRotation) * fSpeed;
		if (isKeyDown('D'))
			nVec->x += cosf(fCameraRotation) * fSpeed, nVec->y -= sinf(fCameraRotation) * fSpeed;
		if (isKeyDown('A'))
			nVec->x -= cosf(fCameraRotation) * fSpeed, nVec->y += sinf(fCameraRotation) * fSpeed;

		if (g_Config.g_Player.bAirBreak) {
			if (isKeyDown(VK_SPACE))
				nVec->z += fSpeed;
			if (isKeyDown(VK_CONTROL))
				nVec->z -= fSpeed;
		}

		if (g_Config.g_Player.bMapRun) {
			const CVector vecTarget = g_p_ped->GetPosition();
			float fPosZ = CWorld::FindGroundZForCoord(vecTarget.x, vecTarget.y) + 1.0f;

			CEntity* pCollisionEntity = nullptr;
			if (CColPoint pCollision; CWorld::ProcessLineOfSight(*nVec,
			                                                     vecTarget,
			                                                     pCollision,
			                                                     pCollisionEntity,
			                                                     true,
			                                                     false,
			                                                     false,
			                                                     true,
			                                                     true,
			                                                     false,
			                                                     false,
			                                                     false))
				fPosZ = pCollision.m_vecPoint.z + 1.0f;

			if (const float fCheckZ = CWorld::FindGroundZForCoord(vecTarget.x, vecTarget.y); fPosZ < fCheckZ)
				fPosZ = fCheckZ + 1.0f;

			float fWaterLevel;
			if (reinterpret_cast<bool(__cdecl*)(float,
			                                    float,
			                                    float,
			                                    float *,
			                                    bool,
			                                    CVector *)>(0x6EB690)(vecTarget.x,
			                                                          vecTarget.y,
			                                                          vecTarget.z,
			                                                          &fWaterLevel,
			                                                          true,
			                                                          new CVector())) {
				if (fPosZ < fWaterLevel)
					fPosZ = fWaterLevel + 1.0f;
			}
			nVec->z = fPosZ;
		}
	}
}

void
Player::AntiStun() {
	if (g_Config.g_Player.bAntiStun && rand() % 100 < g_Config.g_Player.iAntiStunChance)
		g_p_ped->m_nPedFlags.bUpperBodyDamageAnimsOnly = true;
	else g_p_ped->m_nPedFlags.bUpperBodyDamageAnimsOnly = false;
}
