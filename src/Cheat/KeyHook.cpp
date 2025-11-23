#include "Cheat/KeyHook.h"

#include "Utils.h"
#include "Config/Config.h"
#include "Menu/Menu.h"
#include "sampapi/0.3.7-R3-1/CInput.h"

CKeyHook* pKeyHook;

BYTE __stdcall CKeyHook::hkCPad_UpdateGameKey(const int iKey) {
	auto* i16KeyState = reinterpret_cast<int16_t *>(0xB73458);
	for (int i = 0; i < 32; i++) {
		if (pKeyHook->g_GameKeyState[i].bActive) {
			i16KeyState[i] = pKeyHook->g_GameKeyState[i].iState;
			pKeyHook->g_GameKeyState[i].bActive = false;
		}
	}
	return pKeyHook->oCPad_UpdateGameKey(reinterpret_cast<CPad *>(0xB7358C), iKey);
}

extern LRESULT
ImGui_ImplWin32_WndProcHandler(HWND wnd,
                               UINT umsg,
                               WPARAM wparam,
                               LPARAM lparam);

LRESULT APIENTRY CKeyHook::hkWndProc(HWND hWnd,
                                     const UINT msg,
                                     const WPARAM wParam,
                                     const LPARAM lParam) {
	if (Utils::isGTAMenuActive())
		return pKeyHook->oWndProc(hWnd, msg, wParam, lParam);

	switch (msg) {
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		pKeyHook->bKeyTable[VK_LBUTTON] = (msg == WM_LBUTTONDOWN);
		break;

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		pKeyHook->bKeyTable[VK_RBUTTON] = (msg == WM_RBUTTONDOWN);
		break;

	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		pKeyHook->bKeyTable[VK_MBUTTON] = (msg == WM_MBUTTONDOWN);
		break;

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP: {
		const bool bDown = (msg == WM_SYSKEYDOWN || msg == WM_KEYDOWN);
		const int iKey = static_cast<int>(wParam);
		const uint32_t ScanCode = LOBYTE(HIWORD(lParam));

		pKeyHook->bKeyTable[iKey] = bDown;

		switch (iKey) {
		case VK_SHIFT:
			if (ScanCode == MapVirtualKey(VK_LSHIFT, 0)) pKeyHook->bKeyTable[VK_LSHIFT] = bDown;
			if (ScanCode == MapVirtualKey(VK_RSHIFT, 0)) pKeyHook->bKeyTable[VK_RSHIFT] = bDown;
			break;

		case VK_CONTROL:
			if (ScanCode == MapVirtualKey(VK_LCONTROL, 0)) pKeyHook->bKeyTable[VK_LCONTROL] = bDown;
			if (ScanCode == MapVirtualKey(VK_RCONTROL, 0)) pKeyHook->bKeyTable[VK_RCONTROL] = bDown;
			break;

		case VK_MENU:
			if (ScanCode == MapVirtualKey(VK_LMENU, 0)) pKeyHook->bKeyTable[VK_LMENU] = bDown;
			if (ScanCode == MapVirtualKey(VK_RMENU, 0)) pKeyHook->bKeyTable[VK_RMENU] = bDown;
			break;
		default: break;
		}
		break;
	}
	default: break;
	}

	if (pMenu->bOpen)
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

	if (isKeyPressed(VK_HOME)) {
		pMenu->bOpen = !pMenu->bOpen;
		pMenu->iToggle = 0;
	}

	if (!Utils::isGTAMenuActive() && !sampapi::v037r3::RefInputBox()->m_bEnabled) {
		if (g_Config.g_Player.bAirBreak || g_Config.g_Player.bMapRun)
			if (wParam == 'W' || wParam == 'A' || wParam == 'S' || wParam == 'D')
				return pKeyHook->oWndProc(hWnd, WM_KEYUP, wParam, lParam);
	}

	return pKeyHook->oWndProc(hWnd, msg, wParam, lParam);
}

bool
isKeyDown(const uint8_t key) {
	return pKeyHook->bKeyTable[key];
}

bool
isKeyReleased(const uint8_t vkey) {
	static bool bPressed[0xFF];
	if (!pKeyHook->bKeyTable[vkey]) {
		if (bPressed[vkey])
			return !((bPressed[vkey] = false));
	} else bPressed[vkey] = true;

	return false;
}

bool
isKeyPressed(const uint8_t key) {
	static bool bPressed[0xFF];
	if (pKeyHook->bKeyTable[key]) {
		if (!bPressed[key])
			return bPressed[key] = true;
	} else bPressed[key] = false;

	return false;
}
