#include "dat.hpp"
#include "MR.hpp"
#include <shlwapi.h>

int main(int argc, char **argv)
{
    if (argc == 2 && lstrcmpA(argv[1], "--version") == 0)
    {
        puts("dat_limited version 0.0");
        return 0;
    }
    if (argc <= 3 || lstrcmpA(argv[1], "--help") == 0)
    {
        fprintf(stderr, "Usage: dat_limited your_file.dll input.dat output.dat\n");
        return 0;
    }

    HINSTANCE hinst = LoadLibraryA(argv[1]);
    if (!hinst)
    {
        fprintf(stderr, "ERROR: Cannot load DLL file '%s'\n", argv[1]);
        return 3;
    }

    char path[MAX_PATH];
    lstrcpynA(path, argv[1], MAX_PATH);
    PathRemoveExtensionA(path);
    CharLowerA(path);
    char *pchFileName = PathFindFileNameA(path);

    MR_IMPORTS imports;
    MR_get_imports(imports, hinst);

    MR_EXPORTS exports;
    MR_get_exports(exports, pchFileName, hinst);

    MR_DELAY_IMPORTS delays;
    MR_get_delay_imports(delays, hinst);

    std::vector<DataEntry> entries;
    if (!DAT_load(argv[2], entries))
    {
        FreeLibrary(hinst);
        return 1;
    }

    std::vector<DataEntry> new_entries;
    for (auto& entry : entries)
    {
        bool found = false;

        if (!found)
        {
            for (auto& imp : imports)
            {
                if (imp.func_name == entry.func_name)
                {
                    found = true;
                    break;
                }
            }
        }
        if (!found)
        {
            for (auto& exp : exports)
            {
                if (exp.func_name == entry.func_name)
                {
                    found = true;
                    break;
                }
            }
        }
        if (!found)
        {
            for (auto& del : delays)
            {
                if (del.func_name == entry.func_name)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
            new_entries.push_back(entry);
    }

    if (!DAT_save(argv[3], new_entries))
    {
        FreeLibrary(hinst);
        return 2;
    }

    FreeLibrary(hinst);
    return 0;
}
