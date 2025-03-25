#ifndef MY_HEADER_H
#define MY_HEADER_H

namespace WindowsAPI {
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

}

// Your declarations that rely on Windows API types or functions
using WindowsAPI::MEMORY_BASIC_INFORMATION;
using WindowsAPI::VirtualQuery;

#endif // MY_HEADER_H
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
    static auto pattern_to_byte = [](const char* pattern, size_t len) {
        auto bytes = std::vector<int>{};
        const auto start = const_cast<char*>(pattern);
        const auto end = const_cast<char*>(pattern) + len;

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?') ++current;
                bytes.push_back(-1);
            }
            else bytes.push_back(strtoul(current, &current, 16));
        }
        return bytes;
        };

    const auto dos_header = (PIMAGE_DOS_HEADER)FortniteClientWin64Shipping;
    const auto nt_headers = (PIMAGE_NT_HEADERS)((std::uint8_t*)FortniteClientWin64Shipping + dos_header->e_lfanew);

    const auto module_size = nt_headers->OptionalHeader.SizeOfImage;
    const auto module_data = reinterpret_cast<std::uint8_t*>(FortniteClientWin64Shipping);

    auto pattern_as_bytes = pattern_to_byte(signature.c_str(), signature.length());
    const auto size = pattern_as_bytes.size();
    const auto data = pattern_as_bytes.data();

    uintptr_t base_address = (uintptr_t)FortniteClientWin64Shipping;
    uintptr_t end_address = base_address + module_size;

    while (base_address < end_address) {
        size_t region_size = 0;
        if (is_valid_region_ptr((void*)base_address, &region_size) && region_size) {
            uintptr_t region_end = base_address + region_size;
            for (auto i = base_address; i < region_end - size; ++i) {

                bool found = true;
                for (auto j = 0ul; j < size; ++j) {
                    if (module_data[i - (uintptr_t)FortniteClientWin64Shipping + j] != data[j] && data[j] != -1) {
                        found = false;
                        break;
                    }
                }

                if (found)
                    return rva ? rva_to_va<T>(i, rva) : (T)i;
            }
        }

        base_address += region_size;
    }

    return 0;
}