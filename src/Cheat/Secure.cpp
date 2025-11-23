#include "Secure.h"
#include <detours.h>
#include "Memory.h"

CSecure* pSecure = new CSecure();

void CSecure::Add(const DWORD dwAddress, const BYTE byteSize)
{
	for (auto i = 0; i < byteSize; i++)
	{
		if (isAddressSecured(dwAddress + i) == -1)
		{
			g_Memory.dwAddress = dwAddress + i;
			g_Memory.origByte = *reinterpret_cast<BYTE *>(dwAddress + i);
			vecMemory.push_back(g_Memory);
		}
	}
}

int CSecure::isAddressSecured(const DWORD dwAddress) const {
	for (size_t i = 0; i < vecMemory.size(); i++)
	{
		if (vecMemory[i].dwAddress == dwAddress)
			return i;
	}
	return -1;
}

void CSecure::SDetourAttach(PVOID* ppPointer, PVOID pDetour)
{
	Add(*reinterpret_cast<DWORD *>(ppPointer), 7);
	DetourAttach(ppPointer, pDetour);
}

void CSecure::memcpy_safe(void* _dest, const void* _src, uint32_t len)
{
	Add(reinterpret_cast<DWORD>(_dest), len);
	Memory::memcpy_safe(_dest, _src, len);
}

void CSecure::HookInstallCall(const DWORD dwInstallAddress, const DWORD dwHookFunction)
{
	Add(dwInstallAddress, 5);
	Memory::HookInstallCall(dwInstallAddress, dwHookFunction);
}

void CSecure::CreateJump(BYTE* pAddress, const DWORD dwJumpTo, const DWORD dwLen)
{
	Add(reinterpret_cast<DWORD>(pAddress), static_cast<BYTE>(dwLen));
	Memory::CreateJump(pAddress, dwJumpTo, dwLen);
}