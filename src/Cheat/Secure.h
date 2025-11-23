#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>
class CSecure
{
public:
	void Add(DWORD dwAddress, BYTE byteSize);
	int isAddressSecured(DWORD dwAddress) const;
	void SDetourAttach(PVOID* ppPointer, PVOID pDetour);
	void memcpy_safe(void* _dest, const void* _src, uint32_t len);
	void HookInstallCall(DWORD dwInstallAddress, DWORD dwHookFunction);
	void CreateJump(BYTE* pAddress, DWORD dwJumpTo, DWORD dwLen);

	template<class T>
	void Write(DWORD dwAddress, T Value)
	{
		Add(dwAddress, sizeof(Value));
		*reinterpret_cast<T *>(dwAddress) = static_cast<T>(Value);
	}

	struct stMemory
	{
		DWORD dwAddress;
		BYTE origByte;
	} g_Memory;

	std::vector<stMemory> vecMemory;
};

extern CSecure* pSecure;