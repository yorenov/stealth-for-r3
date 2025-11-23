
#include "D3D/D3DHook.h"

#include <imgui_impl_dx9.h>
#include <rakhook/rakhook.hpp>

#include "cheat/KeyHook.h"
#include "cheat/Textures.h"
#include "imgui/imgui_render.h"
#include "cheat/Cheat.h"
#include "Menu/Menu.h"

CD3DHook* pD3DHook;

HRESULT
__stdcall CD3DHook::hkPresent(IDirect3DDevice9* pDevice,
                              CONST RECT* pSrcRect,
                              CONST RECT* pDestRect,
                              HWND hDestWindow,
                              CONST RGNDATA* pDirtyRegion) {
	if (!pDevice)
		return pD3DHook->oPresent(pDevice, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);

	if (rakhook::initialized) {
		if (!pD3DHook->bInit) {
			pD3DHook->pD3DDevice = pDevice;
			pKeyHook = new CKeyHook();
			pTextures = new CTextures(pDevice);
			pMenu = new CMenu(pDevice);
			pRender = new CRender();
			pD3DHook->bInit = true;
		}

		Cheat::Update();
		Cheat::Render();
	}

	return pD3DHook->oPresent(pDevice, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
}

HRESULT
__stdcall CD3DHook::hkReset(IDirect3DDevice9* pDevice,
                            D3DPRESENT_PARAMETERS* pPresentParams) {
	if (!pDevice)
		return pD3DHook->oReset(pDevice, pPresentParams);

	if (pD3DHook->bInit)
		ImGui_ImplDX9_InvalidateDeviceObjects();

	return pD3DHook->oReset(pDevice, pPresentParams);
}
