#include "MR.hpp"
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <cstring>

#define MAX_NAME 80

PIMAGE_NT_HEADERS MR_get_nt_header(void *Base)
{
    auto pbBase = reinterpret_cast<uint8_t*>(Base);
    auto pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(Base);
    if (pbBase[0] != 'M' || pbBase[1] != 'Z' || pDosHeader->e_lfanew == 0)
        return NULL;

    return reinterpret_cast<PIMAGE_NT_HEADERS>(pbBase + pDosHeader->e_lfanew);
}

PIMAGE_NT_HEADERS32 MR_get_nt_header32(void *Base)
{
    return reinterpret_cast<PIMAGE_NT_HEADERS32>(MR_get_nt_header(Base));
}

PIMAGE_NT_HEADERS64 MR_get_nt_header64(void *Base)
{
    return reinterpret_cast<PIMAGE_NT_HEADERS64>(MR_get_nt_header(Base));
}

void *MR_get_dir_entry_data(void *Base, int iDirEntry, uint32_t *size)
{
    if (size)
        *size = 0;

    auto pbBase = reinterpret_cast<uint8_t*>(Base);
    auto pNtHeaders = MR_get_nt_header(Base);
    auto pFileHeader = &pNtHeaders->FileHeader;

    PIMAGE_DATA_DIRECTORY pDataDir;
    switch (pFileHeader->SizeOfOptionalHeader)
    {
    case sizeof(IMAGE_OPTIONAL_HEADER32):
        {
            auto pOptional32 =
                reinterpret_cast<PIMAGE_OPTIONAL_HEADER32>(&pNtHeaders->OptionalHeader);
            pDataDir = &pOptional32->DataDirectory[iDirEntry];
        }
        break;
    case sizeof(IMAGE_OPTIONAL_HEADER64):
        {
            auto pOptional64 =
                reinterpret_cast<PIMAGE_OPTIONAL_HEADER64>(&pNtHeaders->OptionalHeader);
            pDataDir = &pOptional64->DataDirectory[iDirEntry];
        }
        break;
    default:
        return 0;
    }

    size_t Address = pDataDir->VirtualAddress;
    if (!Address)
        return NULL;
    if (size)
        *size = pDataDir->Size;
    return pbBase + Address;
}

int MR_get_module_bits(void *Base)
{
    auto pbBase = reinterpret_cast<uint8_t*>(Base);
    if (pbBase[0] != 'M' || pbBase[1] != 'Z')
        return 0;

    auto pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(Base);
    if (pDosHeader->e_lfanew == 0)
        return 16;

    auto pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(pbBase + pDosHeader->e_lfanew);
    if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
        return 0;

    auto pFileHeader = &pNtHeaders->FileHeader;
    switch (pFileHeader->SizeOfOptionalHeader)
    {
    case sizeof(IMAGE_OPTIONAL_HEADER32):
        return 32;
    case sizeof(IMAGE_OPTIONAL_HEADER64):
        return 64;
    default:
        return 0;
    }
}

bool MR_get_imports32(MR_IMPORTS& imports, void *hModule)
{
    uint32_t size;
    auto pImports = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(
        MR_get_dir_entry_data(hModule, IMAGE_DIRECTORY_ENTRY_IMPORT, &size));
    if (!pImports)
        return false;

    auto pbMod = reinterpret_cast<uint8_t*>(hModule);
    int ordinal, hint;
    char func_name[MAX_NAME];

    for (; pImports->Characteristics; ++pImports)
    {
        auto module_name = reinterpret_cast<char*>(pbMod + pImports->Name);
        auto pINT = reinterpret_cast<PIMAGE_THUNK_DATA32>(pbMod + pImports->OriginalFirstThunk);
        auto pIAT = reinterpret_cast<PIMAGE_THUNK_DATA32>(pbMod + pImports->FirstThunk);
        while (pINT->u1.AddressOfData && pIAT->u1.Function)
        {
            ordinal = hint = -1;

            if (IMAGE_SNAP_BY_ORDINAL32(pINT->u1.Ordinal))
            {
                ordinal = IMAGE_ORDINAL32(pINT->u1.Ordinal);
                std::snprintf(func_name, _countof(func_name), "%u", ordinal);
            }
            else
            {
                auto pName = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(pbMod + pINT->u1.AddressOfData);
                std::snprintf(func_name, _countof(func_name), "%s", pName->Name);
                hint = pName->Hint;
            }

            auto va = pIAT->u1.Function;
            imports.push_back({ module_name, func_name, va, ordinal, hint });

            ++pIAT;
            ++pINT;
        }
    }

    return true;
}

bool MR_get_imports64(MR_IMPORTS& imports, void *hModule)
{
    uint32_t size;
    auto pImports = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(
        MR_get_dir_entry_data(hModule, IMAGE_DIRECTORY_ENTRY_IMPORT, &size));
    if (!pImports)
        return false;

    auto pbMod = reinterpret_cast<uint8_t*>(hModule);
    int ordinal, hint;
    char func_name[MAX_NAME];

    for (; pImports->Characteristics; ++pImports)
    {
        auto module_name = reinterpret_cast<char*>(pbMod + pImports->Name);
        auto pINT = reinterpret_cast<PIMAGE_THUNK_DATA64>(pbMod + pImports->OriginalFirstThunk);
        auto pIAT = reinterpret_cast<PIMAGE_THUNK_DATA64>(pbMod + pImports->FirstThunk);
        while (pINT->u1.AddressOfData && pIAT->u1.Function)
        {
            ordinal = hint = -1;

            if (IMAGE_SNAP_BY_ORDINAL64(pINT->u1.Ordinal))
            {
                ordinal = IMAGE_ORDINAL64(pINT->u1.Ordinal);
                std::snprintf(func_name, _countof(func_name), "%u", ordinal);
            }
            else
            {
                auto pName = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(pbMod + pINT->u1.AddressOfData);
                std::snprintf(func_name, _countof(func_name), "%s", pName->Name);
                hint = pName->Hint;
            }

            auto va = pIAT->u1.Function;
            imports.push_back({ module_name, func_name, va, ordinal, hint });

            ++pIAT;
            ++pINT;
        }
    }

    return true;
}

bool MR_get_imports(MR_IMPORTS& imports, void *hModule)
{
    switch (MR_get_module_bits(hModule))
    {
    case 16:
    default:
        return false;
    case 32:
        return MR_get_imports32(imports, hModule);
    case 64:
        return MR_get_imports64(imports, hModule);
    }
}

bool MR_get_exports(MR_EXPORTS& exports, const char *module_name, void *hModule)
{
    uint32_t size;
    auto pExports = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(
        MR_get_dir_entry_data(hModule, IMAGE_DIRECTORY_ENTRY_EXPORT, &size));
    if (!pExports)
        return false;

    auto pbMod = reinterpret_cast<uint8_t*>(hModule);
    auto names = reinterpret_cast<uint32_t*>(pbMod + pExports->AddressOfNames);
    auto funcs = reinterpret_cast<uint32_t*>(pbMod + pExports->AddressOfFunctions);
    auto ordinals = reinterpret_cast<uint16_t*>(pbMod + pExports->AddressOfNameOrdinals);
    auto cFuncs = pExports->NumberOfFunctions;
    auto cNames = pExports->NumberOfNames;

    /* with name */
    int hint = 0;
    for (uint32_t iName = 0; iName < cNames; ++iName, ++hint)
    {
        int ordinal = pExports->Base + ordinals[iName];
        auto va = funcs[ordinals[iName]];
        auto name = reinterpret_cast<char*>(pbMod + names[iName]);
        std::string forwarded_to;

        void *ptr = pbMod + va;
        if (pExports <= ptr && ptr <= reinterpret_cast<uint8_t*>(pExports) + size)
        {
            forwarded_to = reinterpret_cast<char *>(ptr);
            va = 0;
        }

        exports.push_back({ module_name, name, va, ordinal, hint, forwarded_to });
    }

    /* nameless */
    hint = -1;
    for (uint32_t iFunc = 0; iFunc < cFuncs; ++iFunc)
    {
        if (funcs[iFunc] == 0)
            continue;

        int ordinal = pExports->Base + iFunc;
        auto va = funcs[iFunc];
        void *name = NULL;
        for (uint32_t iName = 0; iName < pExports->NumberOfNames; ++iName)
        {
            if (ordinals[iName] == iFunc)
            {
                name = pbMod + names[iName];
                break;
            }
        }
        if (name)
            continue;

        char szName[MAX_NAME];
        std::snprintf(szName, _countof(szName), "%u", ordinal);

        std::string forwarded_to;
        void *ptr = pbMod + va;
        if (pExports <= ptr && ptr <= reinterpret_cast<uint8_t*>(pExports) + size)
        {
            forwarded_to = reinterpret_cast<char *>(ptr);
            va = 0;
        }

        exports.push_back({ module_name, szName, va, ordinal, hint, forwarded_to });
    }

    return true;
}

bool MR_get_delay_imports32(MR_DELAY_IMPORTS& imports, void *hModule)
{
    uint32_t size;
    auto pDelays = reinterpret_cast<ImgDelayDescr*>(
        MR_get_dir_entry_data(hModule, IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT, &size));
    if (!pDelays)
        return false;

    auto pbMod = reinterpret_cast<uint8_t*>(hModule);
    for (; pDelays->rvaHmod; ++pDelays)
    {
        auto module_name = reinterpret_cast<char*>(pbMod + pDelays->rvaDLLName);
        auto pINT = reinterpret_cast<PIMAGE_THUNK_DATA32>(pbMod + pDelays->rvaINT);
        auto pIAT = reinterpret_cast<PIMAGE_THUNK_DATA32>(pbMod + pDelays->rvaIAT);
        std::string func_name;
        int ordinal, hint;

        while (pINT->u1.AddressOfData && pIAT->u1.Function)
        {
            ordinal = hint = -1;
            func_name.clear();

            if (IMAGE_SNAP_BY_ORDINAL32(pINT->u1.Ordinal))
            {
                ordinal = IMAGE_ORDINAL32(pINT->u1.Ordinal);
            }
            else
            {
                auto pName =
                    reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(pbMod + pINT->u1.AddressOfData);
                func_name = reinterpret_cast<char*>(pName->Name);
                hint = pName->Hint;
            }

            auto va = pIAT->u1.Function;
            imports.push_back( { module_name, func_name, va, ordinal, hint } );

            ++pINT;
            ++pIAT;
        }
    }

    return true;
}

bool MR_get_delay_imports64(MR_DELAY_IMPORTS& imports, void *hModule)
{
    uint32_t size;
    auto pDelays = reinterpret_cast<ImgDelayDescr*>(
        MR_get_dir_entry_data(hModule, IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT, &size));
    if (!pDelays)
        return false;

    auto pbMod = reinterpret_cast<uint8_t*>(hModule);
    for (; pDelays->rvaHmod; ++pDelays)
    {
        auto module_name = reinterpret_cast<char*>(pbMod + pDelays->rvaDLLName);
        auto pINT = reinterpret_cast<PIMAGE_THUNK_DATA64>(pbMod + pDelays->rvaINT);
        auto pIAT = reinterpret_cast<PIMAGE_THUNK_DATA64>(pbMod + pDelays->rvaIAT);
        std::string func_name;
        int ordinal, hint;

        while (pINT->u1.AddressOfData && pIAT->u1.Function)
        {
            ordinal = hint = -1;
            func_name.clear();

            if (IMAGE_SNAP_BY_ORDINAL64(pINT->u1.Ordinal))
            {
                ordinal = IMAGE_ORDINAL64(pINT->u1.Ordinal);
            }
            else
            {
                auto pName =
                    reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(pbMod + pINT->u1.AddressOfData);
                func_name = reinterpret_cast<char*>(pName->Name);
                hint = pName->Hint;
            }

            auto va = pIAT->u1.Function;
            imports.push_back( { module_name, func_name, va, ordinal, hint } );

            ++pINT;
            ++pIAT;
        }
    }

    return true;
}

bool MR_get_delay_imports(MR_DELAY_IMPORTS& imports, void *hModule)
{
    switch (MR_get_module_bits(hModule))
    {
    case 16:
    default:
        return false;
    case 32:
        return MR_get_delay_imports32(imports, hModule);
    case 64:
        return MR_get_delay_imports64(imports, hModule);
    }
}
