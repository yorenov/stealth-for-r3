#include "Features/Combat.h"

#include <common.h>

#include "Cheat/Config/Config.h"
#include "Cheat/Memory.h"
#include "cheat/Secure.h"

void
Combat::Update() {
	NoReload();
	NoRecoil();
}

void
Combat::NoReload() {
	if (!g_Config.g_Combat.bNoReload)
		return;

	if (int iWeaponID = FindPlayerPed()->m_aWeapons[FindPlayerPed()->m_nActiveWeaponSlot].m_eWeaponType;
		iWeaponID >= 22 && iWeaponID <= 42 && FindPlayerPed()->m_aWeapons[FindPlayerPed()->m_nActiveWeaponSlot].
		m_nAmmoInClip == 1)
		FindPlayerPed()->GiveWeapon(static_cast<eWeaponType>(iWeaponID), 0, false);
}

void
Combat::NoRecoil() {
	if (g_Config.g_Combat.bNoRecoil)
		pSecure->memcpy_safe(reinterpret_cast<void *>(0x8D610F), "\xBE\x00\x00\x00\x00", 5);
	else Memory::memcpy_safe(reinterpret_cast<void *>(0x8D610F), "\xBE\x00\x00\x40\x3F", 5);
}
