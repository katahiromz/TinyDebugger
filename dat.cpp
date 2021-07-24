#include "dat.hpp"
#include <shlwapi.h>

bool DAT_load(const char *filename, std::vector<DataEntry>& entries)
{
    FILE *fin = fopen(filename, "r");
    if (!fin)
    {
        fprintf(stderr, "ERROR: Cannot read file '%s'.\n", filename);
        return false;
    }

    char buf[256];
    while (fgets(buf, _countof(buf), fin))
    {
        char *pch = strchr(buf, ';');
        if (pch)
            *pch = 0;
        pch = strchr(buf, '#');
        if (pch)
            *pch = 0;

        StrTrimA(buf, " \t\r\n");

        DataEntry entry;
        pch = strtok(buf, "\t\r\n");
        while (pch)
        {
            entry.module_name = pch;
            pch = strtok(NULL, "\t\r\n");
            if (pch)
            {
                entry.cc = pch;
                pch = strtok(NULL, "\t\r\n");
                if (pch)
                {
                    entry.func_name = pch;
                    pch = strtok(NULL, "\t\r\n");
                    while (pch)
                    {
                        entry.args.push_back(pch);
                        pch = strtok(NULL, "\t\r\n");
                    }
                }
                entries.push_back(entry);
            }
        }
    }

    fclose(fin);
    return true;
}

bool DAT_save(const char *filename, const std::vector<DataEntry>& entries)
{
    FILE *fout = fopen(filename, "w");
    if (!fout)
    {
        fprintf(stderr, "ERROR: Cannot write file '%s'.\n", filename);
        return false;
    }

    for (auto& entry : entries)
    {
        fprintf(fout, "%s\t%s\t%s",
            entry.module_name.c_str(),
            entry.cc.c_str(),
            entry.func_name.c_str());
        for (auto& arg : entry.args)
        {
            fprintf(fout, "\t%s", arg.c_str());
        }
        fprintf(fout, "\n");
    }

    fclose(fout);
    return true;
}
