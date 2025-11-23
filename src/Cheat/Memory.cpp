#include "Cheat/Memory.h"

#include <cstring>

#include "Features/Visuals.h"

bool Memory::isBadPtr_handlerAny(void* pointer, const ULONG size, const DWORD dwFlags)
{
	MEMORY_BASIC_INFORMATION	meminfo;

	if (nullptr == pointer)
		return true;

	memset(&meminfo, 0x00, sizeof(meminfo));

	if (const DWORD dwSize = VirtualQuery(pointer, &meminfo, sizeof(meminfo)); 0 == dwSize)
		return true;

	if (MEM_COMMIT != meminfo.State)
		return true;

	if (0 == (meminfo.Protect & dwFlags))
		return true;

	if (size > meminfo.RegionSize)
		return true;

	if (static_cast<unsigned>(static_cast<char *>(pointer) - static_cast<char *>(meminfo.BaseAddress)) > static_cast<unsigned>(meminfo.RegionSize - size))
		return true;

	return false;
}

bool Memory::isBadPtr_readAny(void* pointer, const ULONG size)
{
	return isBadPtr_handlerAny(pointer, size, PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
}

bool Memory::isBadPtr_writeAny(void* pointer, const ULONG size)
{
	return isBadPtr_handlerAny(pointer, size, PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
}

int Memory::__page_size_get()
{
	static int	page_size = -1;
	SYSTEM_INFO si;

	if (page_size == -1)
	{
		GetSystemInfo(&si);
		page_size = static_cast<int>(si.dwPageSize);
	}

	return page_size;
}

int Memory::__page_write(void* _dest, const void* _src, uint32_t len)
{
	static int		page_size = __page_size_get();
	auto* dest = static_cast<uint8_t *>(_dest);
	auto src = static_cast<const uint8_t *>(_src);
	DWORD			prot_prev = 0;
	int				prot_changed = 0;
	int				ret = 1;

	while (len > 0)
	{
		ret = 1;
		const int page_offset = static_cast<int>(reinterpret_cast<UINT_PTR>(dest) % page_size);
		const int page_remain = page_size - page_offset;
		int this_len = len;

		if (this_len > page_remain)
			this_len = page_remain;

		if (isBadPtr_writeAny(dest, this_len))
		{
			if (!VirtualProtect((void*)dest, this_len, PAGE_EXECUTE_READWRITE, &prot_prev))
				ret = 0;
			else prot_changed = 1;
		}

		if (ret)
			memcpy(dest, src, this_len);

		if (prot_changed)
		{
			DWORD dummy;
			VirtualProtect((void*)dest, this_len, prot_prev, &dummy);
		}

		dest += this_len;
		src += this_len;
		len -= this_len;
	}

	return ret;
}

int Memory::__page_read(void* _dest, const void* _src, int len)
{
	static int	page_size = __page_size_get();
	auto* dest = static_cast<uint8_t *>(_dest);
	auto* src = (uint8_t*)_src;
	DWORD		prot_prev = 0;
	int			prot_changed = 0;
	int			ret = 1;

	while (len > 0)
	{
		ret = 1;
		const int page_offset = static_cast<int>(reinterpret_cast<UINT_PTR>(src) % page_size);
		const int page_remain = page_size - page_offset;
		int this_len = len;

		if (this_len > page_remain)
			this_len = page_remain;

		if (isBadPtr_readAny(src, this_len))
		{
			if (!VirtualProtect((void*)src, this_len, PAGE_EXECUTE_READWRITE, &prot_prev))
				ret = 0;
			else prot_changed = 1;
		}

		if (ret)
			memcpy(dest, src, this_len);
		else memset(dest, 0, this_len);

		if (prot_changed)
		{
			DWORD	dummy;
			VirtualProtect((void*)src, this_len, prot_prev, &dummy);
		}

		dest += this_len;
		src += this_len;
		len -= this_len;
	}

	return ret;
}

int Memory::memcpy_safe(void* _dest, const void* _src, uint32_t len, const int check, const void* checkdata)
{
	static int		page_size = __page_size_get();
	static int		recurse_ok = 1;
	auto* dest = static_cast<uint8_t *>(_dest);
	const auto* src = static_cast<const uint8_t *>(_src);
	int				ret = 1;
	if (check && checkdata)
	{
		if (!memcmp_safe(checkdata, _dest, len))
			return 0;
	}

	while (len > 0)
	{
		uint8_t buf[4096];
		uint32_t this_len = sizeof(buf);

		if (this_len > len)
			this_len = len;

		if (!__page_read(buf, src, this_len))
			ret = 0;

		if (!__page_write(dest, buf, this_len))
			ret = 0;

		len -= this_len;
		src += this_len;
		dest += this_len;
	}

	return ret;
}

int Memory::memset_safe(void* _dest, const int c, uint32_t len)
{
	auto* dest = static_cast<uint8_t *>(_dest);
	uint8_t buf[4096];

	memset(buf, c, (len > 4096) ? 4096 : len);

	for (;; )
	{
		if (len > 4096)
		{
			if (!memcpy_safe(dest, buf, 4096))
				return 0;
			dest += 4096;
			len -= 4096;
		}
		else
		{
			if (!memcpy_safe(dest, buf, len))
				return 0;
			break;
		}
	}

	return 1;
}

int Memory::memcmp_safe(const void* _s1, const void* _s2, uint32_t len)
{
	const auto* s1 = static_cast<const uint8_t *>(_s1);
	const auto* s2 = static_cast<const uint8_t *>(_s2);

	for (;;) {
		uint8_t buf[4096];
		if (len > 4096)
		{
			if (!memcpy_safe(buf, s1, 4096))
				return 0;
			if (memcmp(buf, s2, 4096) != 0)
				return 0;
			s1 += 4096;
			s2 += 4096;
			len -= 4096;
		}
		else
		{
			if (!memcpy_safe(buf, s1, len))
				return 0;
			if (memcmp(buf, s2, len) != 0)
				return 0;
			break;
		}
	}

	return 1;
}

void* Memory::memdup(const void* src, const int len)
{
	void* dest = malloc(len);

	if (dest != nullptr)
		memcpy(dest, src, len);

	return dest;
}

void Memory::HookInstallCall(const DWORD dwInstallAddress, const DWORD dwHookFunction)
{
	const DWORD dwOffset = dwHookFunction - (dwInstallAddress + 5);
	*reinterpret_cast<BYTE *>(dwInstallAddress) = 0xE8;
	*reinterpret_cast<DWORD *>(dwInstallAddress + 1) = dwOffset;
}

void Memory::CreateJump(BYTE* pAddress, const DWORD dwJumpTo, const DWORD dwLen)
{
	DWORD dwOldProtect, dwBkup;
	VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	const DWORD dwRelAddr = (DWORD) (dwJumpTo - reinterpret_cast<DWORD>(pAddress)) - 5;
	*pAddress = 0xE9;
	*reinterpret_cast<DWORD *>(pAddress + 0x1) = dwRelAddr;
	for (DWORD x = 0x5; x < dwLen; x++)*(pAddress + x) = 0x90;
	VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);
}
