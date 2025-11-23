#pragma once
#include <CVector.h>
#include <ePedBones.h>
#include <CPed.h>

namespace Utils
{
	void CalcScreenCoors(const CVector* vecWorld, CVector* vecScreen);
	void CalcWorldCoors(CVector* vecScreen, CVector* vecWorld);
	int isGTAMenuActive();
	float getDistance(const CVector &vecTarget);
	float getDistanceFromCamera(const CVector &vecTarget);
	void DisembarkInstantly();
	CVector* getBonePosition(CPed* pPed, ePedBones bone, CVector* vecPosition);
	bool isPlayingAnimation(CEntity* pEntity, const char* szAnimName);
	float vecLength(const CVector &vecPos);
}

