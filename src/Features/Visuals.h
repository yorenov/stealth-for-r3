#pragma once
#include <CPed.h>
#include <detours.h>

#include "Menu/Menu.h"
#include "cheat/Secure.h"

class CVisuals {
public:
	CVisuals() {
		fHitmarkerAlpha = 0.f;
		oCalculateAspectRatio = reinterpret_cast<tCalculateAspectRatio>(0x6FF420);
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		pSecure->SDetourAttach(&reinterpret_cast<PVOID &>(oCalculateAspectRatio), hkCalculateAspectRatio);
		DetourTransactionCommit();
	}

	~CVisuals() {
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&reinterpret_cast<PVOID &>(oCalculateAspectRatio), hkCalculateAspectRatio);
		DetourTransactionCommit();
	}

	void
	Render();

	static bool
	isPlayerVisible(int iPlayerID,
	                bool bDistance);
	static ImColor
	getESPColor(int iPlayerID);

	static void
	ESP_NameTags(int iPlayerID,
	             CPed* pPed);

	static void
	ESP_Snipeline(int iPlayerID,
	              CPed* pPed);

	static void
	ESP_Box(int iPlayerID,
	        CPed* pPed);

	static void
	ESP_Bones(int iPlayerID,
	          CPed* pPed);

	static void
	ESP_Chams(int iPlayerID,
	          CPed* pPed);

	static void
	ESP_Distance(int iPlayerID,
	             CPed* pPed);

	struct stDMGInformer {
		CVector vecPos = CVector(0, 0, 0);
		int iWeapon = 0;
		ULONGLONG dwTick = 0;
	} g_DMGInformer[10];

	float fHitmarkerAlpha;

private:
	typedef float
	(__cdecl*tCalculateAspectRatio)();
	static float
	__cdecl hkCalculateAspectRatio();
	tCalculateAspectRatio oCalculateAspectRatio;
};

extern CVisuals* pVisuals;
