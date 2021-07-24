#include "spec.hpp"
#include <shlwapi.h>
#include <strsafe.h>
#include "mstr.hpp"

bool do_write(const std::string& output, SPEC& spec, std::vector<std::string>& spec_files)
{
    FILE *fout = fopen(output.c_str(), "w");
    if (!fout)
    {
        fprintf(stderr, "ERROR: Cannot write file '%s'\n", output.c_str());
        return false;
    }

    fprintf(fout, "; spec2wizc %s", output.c_str());
    for (auto& file : spec_files)
    {
        fprintf(fout, " %s", file.c_str());
    }
    fprintf(fout, "\n");

    for (auto& pair : spec.func_name_to_item)
    {
        auto& item = pair.second;
        fprintf(fout, "%s\t%s\t%s", item.module_name.c_str(),
                item.cc.c_str(), pair.first.c_str());
        for (auto& arg : item.args)
        {
            fprintf(fout, "\t%s", arg.c_str());
        }
        fprintf(fout, "\n");
    }

    fclose(fout);
    return true;
}

int main(int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "--version") == 0)
    {
        puts("spec2dat 0.0 by katahiromz");
        return 0;
    }
    if (argc <= 2)
    {
        puts("Usage: spec2dat data.dat file1.spec file2.spec ...");
        return 0;
    }

    std::string output = argv[1];
    std::vector<std::string> spec_files;
    for (int i = 2; i < argc; ++i)
    {
        spec_files.push_back(argv[i]);
    }

    SPEC spec;
    for (auto& file : spec_files)
    {
        FILE *fp = fopen(file.c_str(), "r");
        if (!fp)
        {
            fprintf(stderr, "ERROR: Cannot read file '%s'\n", file.c_str());
            return 1;
        }

        char pathname[MAX_PATH];
        StringCbCopyA(pathname, sizeof(pathname), file.c_str());
        PathRemoveExtensionA(pathname);
        _strlwr(pathname);
        char *module_name = PathFindFileNameA(pathname);

        if (!SPEC_read(spec, module_name, fp))
        {
            fprintf(stderr, "ERROR: Bad syntax: %s\n", file.c_str());
            return 1;
        }

        fclose(fp);
    }

    if (!do_write(output, spec, spec_files))
        return 1;

    return 0;
}
