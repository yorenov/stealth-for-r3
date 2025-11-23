#pragma once
#include <cstdint>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include "main.h"
#include "cheat/Icons.h"
#include <Windows.h>
#include <d3d9.h>

class CMenu
{
public:
	explicit CMenu(IDirect3DDevice9* pDevice)
	{
		iToggle = 2;
		iCurrentTab = 0;
		iCurrentCategory = 0;
		bOpen = false;
		ImGui::CreateContext();
		ImGui::GetIO().IniFilename = nullptr;
		ImGui_ImplWin32_Init(GetActiveWindow());
		ImGui_ImplDX9_Init(pDevice);
		pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\verdanab.ttf)", 13.1f);
		pIcons = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(uiIconData, uiIconSize, 30);
		Theme();
	};

	~CMenu();

	void Update();
	void Render();

	static void Theme();

	int iToggle, iCurrentTab, iCurrentCategory;
	bool bOpen;

	ImFont* pFont, *pIcons;

	static void showCursor(const bool state) {
	    using RwD3D9GetCurrentD3DDevice_t = LPDIRECT3DDEVICE9(__cdecl *)();

	    const auto rwCurrentD3dDevice{reinterpret_cast<
	        RwD3D9GetCurrentD3DDevice_t>(0x7F9D50U)()};

	    if (nullptr == rwCurrentD3dDevice) {
	        return;
	    }

	    static DWORD
	        updateMouseProtection,
	        rsMouseSetPosProtFirst,
	        rsMouseSetPosProtSecond;

	    if (state)
	    {
	        VirtualProtect(reinterpret_cast<void *>(0x53F3C6U), 5U,
	            PAGE_EXECUTE_READWRITE, &updateMouseProtection);

	        VirtualProtect(reinterpret_cast<void *>(0x53E9F1U), 5U,
	            PAGE_EXECUTE_READWRITE, &rsMouseSetPosProtFirst);

	        VirtualProtect(reinterpret_cast<void *>(0x748A1BU), 5U,
	            PAGE_EXECUTE_READWRITE, &rsMouseSetPosProtSecond);

	        // NOP: CPad::UpdateMouse
	        *reinterpret_cast<uint8_t *>(0x53F3C6U) = 0xE9U;
	        *reinterpret_cast<uint32_t *>(0x53F3C6U + 1U) = 0x15BU;

	        // NOP: RsMouseSetPos
	        memset(reinterpret_cast<void *>(0x53E9F1U), 0x90, 5U);
	        memset(reinterpret_cast<void *>(0x748A1BU), 0x90, 5U);

	        rwCurrentD3dDevice->ShowCursor(TRUE);
	    }
	    else
	    {
	        // Original: CPad::UpdateMouse
	        memcpy(reinterpret_cast<void *>(0x53F3C6U), "\xE8\x95\x6C\x20\x00", 5U);

	        // Original: RsMouseSetPos
	        memcpy(reinterpret_cast<void *>(0x53E9F1U), "\xE8\xAA\xAA\x0D\x00", 5U);
	        memcpy(reinterpret_cast<void *>(0x748A1BU), "\xE8\x80\x0A\xED\xFF", 5U);

	        using CPad_ClearMouseHistory_t = void(__cdecl *)();
	        using CPad_UpdatePads_t = void(__cdecl *)();

	        reinterpret_cast<CPad_ClearMouseHistory_t>(0x541BD0U)();
	        reinterpret_cast<CPad_UpdatePads_t>(0x541DD0U)();

	        VirtualProtect(reinterpret_cast<void *>(0x53F3C6U), 5U,
	            updateMouseProtection, &updateMouseProtection);

	        VirtualProtect(reinterpret_cast<void *>(0x53E9F1U), 5U,
	            rsMouseSetPosProtFirst, &rsMouseSetPosProtFirst);

	        VirtualProtect(reinterpret_cast<void *>(0x748A1BU), 5U,
	            rsMouseSetPosProtSecond, &rsMouseSetPosProtSecond);

	        rwCurrentD3dDevice->ShowCursor(FALSE);
	    }
	}
private:
	static void ActiveTab();

	static void NormalTab();

	static void ActiveButton();

	static void NormalButton();

	static void Tooltip(const char* szText);

};

extern CMenu* pMenu;
