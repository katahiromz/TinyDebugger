#pragma once

#ifdef _WIN32
    #include <windows.h>
    #include <delayimp.h>
#else
    #include "won32.h"
#endif
#include <string>
#include <vector>

PIMAGE_NT_HEADERS MR_get_nt_header(void *Base);
PIMAGE_NT_HEADERS32 MR_get_nt_header32(void *Base);
PIMAGE_NT_HEADERS64 MR_get_nt_header64(void *Base);

void *MR_get_dir_entry_data(void *Base, int iDirEntry, uint32_t *size = NULL);
int MR_get_module_bits(void *Base);

struct MR_IMPORT_ITEM
{
    std::string module_name;
    std::string func_name;
    union { uint64_t func_va; uint32_t func_va32; };
    int ordinal; /* -1 for invalid */
    int hint; /* -1 for invalid */
};
typedef std::vector<MR_IMPORT_ITEM> MR_IMPORTS;

bool MR_get_imports(MR_IMPORTS& imports, void *hModule);
bool MR_get_imports32(MR_IMPORTS& imports, void *hModule);
bool MR_get_imports64(MR_IMPORTS& imports, void *hModule);

struct MR_EXPORT_ITEM
{
    std::string module_name;
    std::string func_name;
    union { uint64_t func_va; uint32_t func_va32; };
    int ordinal; /* -1 for invalid */
    int hint; /* -1 for invalid */
    std::string forwarded_to;
};
typedef std::vector<MR_EXPORT_ITEM> MR_EXPORTS;

bool MR_get_exports(MR_EXPORTS& exports, const char *module_name, void *hModule);

struct MR_DELAY_IMPORT_ITEM
{
    std::string module_name;
    std::string func_name;
    union { uint64_t func_va; uint32_t func_va32; };
    int ordinal; /* -1 for invalid */
    int hint; /* -1 for invalid */
};
typedef std::vector<MR_DELAY_IMPORT_ITEM> MR_DELAY_IMPORTS;

bool MR_get_delay_imports(MR_DELAY_IMPORTS& imports, void *hModule);
bool MR_get_delay_imports32(MR_DELAY_IMPORTS& imports, void *hModule);
bool MR_get_delay_imports64(MR_DELAY_IMPORTS& imports, void *hModule);
