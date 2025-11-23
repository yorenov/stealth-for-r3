#pragma once
#include "Features/Aimbot.h"
#include <d3d9.h>

class CD3DHook
{
public:
	CD3DHook(): pD3DDevice(nullptr) {
		bInit = false;
		oPresent = reinterpret_cast<tPresent>(GetDeviceAddress(17));
		oReset = reinterpret_cast<tReset>(GetDeviceAddress(16));
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		pSecure->SDetourAttach(&reinterpret_cast<PVOID &>(oPresent), hkPresent);
		pSecure->SDetourAttach(&reinterpret_cast<PVOID &>(oReset), hkReset);
		DetourTransactionCommit();
	}
	;

	~CD3DHook()
	{
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)oPresent, hkPresent);
		DetourDetach(&(PVOID&)oReset, hkReset);
		DetourTransactionCommit();
	};

	bool bInit;
	IDirect3DDevice9* pD3DDevice;

private:
	static DWORD FindDevice(const DWORD dwLen)
	{
		DWORD dwObjBase = 0;
		char infoBuf[MAX_PATH];
		GetSystemDirectoryA(infoBuf, MAX_PATH);
		strcat_s(infoBuf, MAX_PATH, "\\d3d9.dll");
		dwObjBase = reinterpret_cast<DWORD>(LoadLibraryA(infoBuf));
		while (dwObjBase++ < dwObjBase + dwLen)
		{
			if (*reinterpret_cast<WORD *>(dwObjBase + 0x00) == 0x06C7 &&
				*reinterpret_cast<WORD *>(dwObjBase + 0x06) == 0x8689 &&
				*reinterpret_cast<WORD *>(dwObjBase + 0x0C) == 0x8689)
			{
				dwObjBase += 2;
				break;
			}
		}
		return(dwObjBase);
	};

	static DWORD GetDeviceAddress(const int VTableIndex)
	{
		PDWORD VTable;
		*reinterpret_cast<DWORD *>(&VTable) = *reinterpret_cast<DWORD *>(FindDevice(0x128000));
		return VTable[VTableIndex];
	};

	typedef HRESULT(__stdcall* tPresent)(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
	typedef HRESULT(__stdcall* tReset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

	static HRESULT __stdcall hkPresent(IDirect3DDevice9* pDevice, CONST RECT* pSrcRect, CONST RECT* pDestRect, HWND hDestWindow, CONST RGNDATA* pDirtyRegion);
	static HRESULT __stdcall hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentParams);

	tPresent oPresent;
	tReset oReset;
};

extern CD3DHook* pD3DHook;