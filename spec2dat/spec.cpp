#include "spec.hpp"
#include <cctype>
#include <cstring>
#include <cstdio>
#include "mstr.hpp"

void print_ignored_line(std::string& line, int num)
{
    mstr_trim(line, " \t\r\n");
    if (line.empty())
        return;
    fprintf(stderr, "WARNING: ignored(%d): %s\n", num, line.c_str());
}

bool SPEC_read(SPEC& spec, const char *module_name, FILE *fin)
{
    char buf[256];
    bool ok = true;

    while (fgets(buf, sizeof(buf), fin))
    {
        if (char *pch = std::strchr(buf, '#'))
            *pch = 0;
        if (char *pch = std::strchr(buf, ';'))
            *pch = 0;

        std::string line = buf;

        if (char *pch = std::strstr(buf, "-stub"))
        {
            print_ignored_line(line, 1);
            continue;
        }
        if (char *pch = std::strstr(buf, "-arch"))
        {
            print_ignored_line(line, 1);
            continue;
        }

        if (std::strchr(buf, '(') == NULL || std::strchr(buf, ')') == NULL ||
            std::strstr(buf, "__wine_") != NULL)
        {
            print_ignored_line(line, 2);
            continue;
        }

        char *pch = buf;
        while (std::isspace(*pch))
            ++pch;

        if (*pch != '@' && !std::isdigit(*pch))
        {
            print_ignored_line(line, 3);
            continue;
        }

        if (*pch == '@')
        {
            ++pch;
        }
        else
        {
            while (std::isdigit(*pch))
                ++pch;
        }

        while (std::isspace(*pch))
            ++pch;

        pch = std::strtok(pch, " \t\r\n");
        if (pch == NULL)
        {
            print_ignored_line(line, 4);
            continue;
        }

        std::string cc;
        if (std::strcmp(pch, "stdcall") == 0)
        {
            cc = "stdcall";
        }
        else if (std::strcmp(pch, "cdecl") == 0)
        {
            cc = "cdecl";
        }
        else
        {
            print_ignored_line(line, 5);
            continue;
        }

        pch = std::strtok(NULL, " \t()\r\n");
        if (pch == NULL)
        {
            print_ignored_line(line, 6);
            continue;
        }

        if (strchr(pch, '?') != NULL || strchr(pch, '-') != NULL)
        {
            print_ignored_line(line, 7);
            continue;
        }

        SPEC_ITEM item;
        item.module_name = module_name;
        item.func_name = pch;
        item.cc = cc;
        for (;;)
        {
            pch = std::strtok(NULL, " \t()\r\n");
            if (!pch)
                break;

            std::string arg = pch;
            if (arg == "long" || arg == "ptr" || arg == "str" || arg == "wstr" ||
                arg == "double")
            {
                item.args.push_back(arg);
            }
            else
            {
                print_ignored_line(line, 8);
                continue;
            }
        }

        spec.func_name_to_item[item.func_name] = item;
        spec.modules.insert(module_name);

        mstr_trim(line, " \t\r\n");
        fprintf(stderr, "passed: %s\n", line.c_str());
    }

    return ok;
}
