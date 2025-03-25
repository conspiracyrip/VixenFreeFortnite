#ifndef MY_HEADER_H
#define MY_HEADER_H

namespace WindowsAPI {
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

}
#endif // MY_HEADER_H


using WindowsAPI::MEMORY_BASIC_INFORMATION;
using WindowsAPI::VirtualQuery;

template <typename T = uintptr_t>
static T rva_to_va(uintptr_t address, int size) { return address ? (T)(address + *(int*)(address + size - 4) + size) : T(); }
static bool is_valid_region_ptr(void* ptr, size_t* out_region_size = nullptr)
{
    MEMORY_BASIC_INFORMATION mbi{};
    if (!VirtualQuery(ptr, &mbi, sizeof(mbi)))
        return false;

    if (out_region_size)
        *out_region_size = mbi.RegionSize;

    return (!(mbi.Protect & PAGE_GUARD) && mbi.Protect != PAGE_NOACCESS && mbi.State == MEM_COMMIT &&
        mbi.Protect & (PAGE_READONLY | PAGE_EXECUTE_READ | PAGE_READWRITE | PAGE_EXECUTE_READWRITE));
}

template <typename T = uintptr_t>
static T find_pattern(const std::string& signature, int rva = 0)
{
    static auto pattern_to_byte = [](const char* pattern, size_t len, int*& outBytes, size_t& outSize) {
        size_t maxSize = len / 2 + 1; // Worst-case size
        int* bytes = new int[maxSize];
        size_t size = 0;

        const auto start = const_cast<char*>(pattern);
        const auto end = const_cast<char*>(pattern) + len;

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?') ++current;
                bytes[size++] = -1;
            }
            else {
                bytes[size++] = strtoul(current, &current, 16);
            }
        }

        outBytes = bytes;
        outSize = size;
        };

    const auto dos_header = (PIMAGE_DOS_HEADER)FortniteClientWin64Shipping;
    const auto nt_headers = (PIMAGE_NT_HEADERS)((std::uint8_t*)FortniteClientWin64Shipping + dos_header->e_lfanew);

    const auto module_size = nt_headers->OptionalHeader.SizeOfImage;
    const auto module_data = reinterpret_cast<std::uint8_t*>(FortniteClientWin64Shipping);

    int* pattern_as_bytes = nullptr;
    size_t size = 0;

    // Convert pattern to bytes
    pattern_to_byte(signature.c_str(), signature.length(), pattern_as_bytes, size);

    const auto data = pattern_as_bytes;

    uintptr_t base_address = (uintptr_t)FortniteClientWin64Shipping;
    uintptr_t end_address = base_address + module_size;

    while (base_address < end_address) {
        size_t region_size = 0;
        if (is_valid_region_ptr((void*)base_address, &region_size) && region_size) {
            uintptr_t region_end = base_address + region_size;
            for (auto i = base_address; i < region_end - size; ++i) {

                bool found = true;
                for (size_t j = 0; j < size; ++j) {
                    if (module_data[i - (uintptr_t)FortniteClientWin64Shipping + j] != data[j] && data[j] != -1) {
                        found = false;
                        break;
                    }
                }

                if (found) {
                    delete[] pattern_as_bytes; // Clean up
                    return rva ? rva_to_va<T>(i, rva) : (T)i;
                }
            }
        }

        base_address += region_size;
    }

    delete[] pattern_as_bytes; // Clean up
    return 0;
}
