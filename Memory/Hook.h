
void* TrampolineHook(BYTE* src, BYTE* dst, size_t size)
{
	
	BYTE* newAlloc = (BYTE*)WindowsAPI::VirtualAlloc(0, size + 10, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!newAlloc)
		return NULL;

	uintptr_t newAddr = (uintptr_t)(src + size);
	WindowsAPI::DWORD relAddr = (DWORD)(dst - src - 5);

	DWORD gatewayProtect;
	if (WindowsAPI::VirtualProtect(newAlloc, size, PAGE_EXECUTE_READWRITE, &gatewayProtect))
	{
		memcpy(newAlloc, src, size);
		memcpy((BYTE*)(newAlloc + size), (BYTE*)"\x48\xBA", 2);
		memcpy((BYTE*)(newAlloc + size + 2), &newAddr, 8);
		memcpy((BYTE*)(newAlloc + size + 10), (BYTE*)"\xFF\xE2", 2);
		WindowsAPI::VirtualProtect(newAlloc, size, gatewayProtect, &gatewayProtect);
	}

	DWORD oldProtect;
	if (WindowsAPI::VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		*src = 0xE9;
		*(DWORD*)(src + 1) = relAddr;
		WindowsAPI::VirtualProtect(src, size, oldProtect, &oldProtect);
	}

	return newAlloc;
}