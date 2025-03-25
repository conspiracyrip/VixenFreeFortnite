#pragma once

class Scanner
{
private:
	DWORD LoopCount;

	PIMAGE_NT_HEADERS Sponge(uintptr_t Moudle)
	{
		return (PIMAGE_NT_HEADERS)((BYTE*)Moudle + PIMAGE_DOS_HEADER(Moudle)->e_lfanew);
	}

	bool CheckMask(BYTE* buffer, const char* pattern, const char* mask)
	{
		for (; *mask; pattern++, mask++, buffer++) 
		{
			if (*mask == '?' || *buffer == *(BYTE*)pattern)
				continue;
			return false;
		}

		return true;
	}

	BYTE* Mask(BYTE* start, DWORD size, const char* pattern, const char* mask)
	{
		for (auto x = 0; x < size - __strlen(mask); x++)
		{
			BYTE* addr = (BYTE*)start + x;
			if (CheckMask(addr, pattern, mask))
				return addr;
		}

		return nullptr;
	}
public:
	BYTE* Scan(uintptr_t mxdule, const char* pattern, const char* mask, DWORD occurence)
	{
		size_t size = Sponge(mxdule)->OptionalHeader.SizeOfImage;

		if (occurence) occurence -= 1;
		for (int x = 0; x < size - __strlen(mask); x++)
		{
			BYTE* addr = (BYTE*)mxdule + x;

			if (CheckMask(addr, pattern, mask))
			{
				if (!occurence || LoopCount == occurence && addr)
				{
					LoopCount = 0; // resets the count. (Incase we need to use it again)
					return addr;
				}

				if (LoopCount < occurence) 
				{
					addr = Mask((BYTE*)mxdule + x, (BYTE*)size - addr, pattern, mask);
					++LoopCount;
					continue;
				}
			}
		}

		return nullptr;
	}
};