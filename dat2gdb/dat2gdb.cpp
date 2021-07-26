#include "dat.hpp"

std::string arg2gdbtype(std::string& arg)
{
    if (arg == "str")
        return "char*";
    if (arg == "wstr")
        return "short*";
    if (arg == "long")
        return "int";
    if (arg == "ptr")
        return "int";
    if (arg == "double")
        return "double";
    return "";
}

BOOL IsForbiddenFunction(std::string& func_name)
{
    return func_name == "CharUpperW" ||
           func_name == "LoadIconW" ||
           func_name == "LoadCursorW" ||
           func_name == "CreateWindowExW";
}

int main(int argc, char **argv)
{
    if (argc <= 1 || strcmp(argv[1], "--help") == 0)
    {
        puts("Usage: dat2gdb data.dat command.gdb");
        return 0;
    }

    if (strcmp(argv[1], "--version") == 0)
    {
        puts("dat2gdb version 0.0 by katahiromz");
        return 0;
    }

    std::vector<DataEntry> entries;
    if (DAT_load(argv[1], entries))
    {
        if (FILE *fout = fopen(argv[2], "w"))
        {
            fputs(
                "set breakpoint pending on\n"
                "set pagination off\n"
                "set logging file log.txt\n"
                "set logging on\n"
                "\n"
                "define ac_print\n"
                "  set $c = (char*)$arg0\n"
                "  if {int}$c == 0\n"
                "    printf \"NULL\"\n"
                "  else\n"
                "    if ({int}$c & 0xFFFF0000) == 0\n"
                "      printf \"0x%X\", {int}$c\n"
                "    else\n"
                "      printf \"'%s'\", $c\n"
                "    end\n"
                "  end\n"
                "end\n"
                "\n"
                "define wc_print\n"
                "  set $c = (short*)$arg0\n"
                "  if {int}$c == 0\n"
                "    printf \"NULL\"\n"
                "  else\n"
                "    if ({int}$c & 0xFFFF0000) == 0\n"
                "      printf \"0x%X\", {int}$c\n"
                "    else\n"
                "      printf \"'\"\n"
                "      while (*$c)\n"
                "        if (*$c > 0x7f)\n"
                "          printf \"[%x]\", *$c\n"
                "        else\n"
                "          if (*$c < 0x20)\n"
                "            printf \"[%x]\", *$c\n"
                "          else\n"
                "            printf \"%c\", *$c\n"
                "          end\n"
                "        end\n"
                "        set $c++\n"
                "      end\n"
                "      printf \"'\"\n"
                "    end\n"
                "  end\n"
                "end\n"
                "\n", fout);

            for (auto& entry : entries)
            {
                auto& args = entry.args;
                fprintf(fout, "break %s\n", entry.func_name.c_str());
                fprintf(fout,
                    "commands\n"
                    "  silent\n");

                auto& func_name = entry.func_name;
                if (IsForbiddenFunction(func_name))
                {
                    fprintf(fout, "  printf \"log: %s(...\"\n", func_name.c_str());
                }
                else
                {
                    for (size_t i = 0; i < args.size(); ++i)
                    {
                        auto& arg = args[i];
                        fprintf(fout, "  set $p%d = {%s}($sp+0x%X)\n", (int)i,
                            arg2gdbtype(arg).c_str(), i * 4 + 8);
                    }

                    fprintf(fout, "  printf \"log: %s(\"\n", func_name.c_str());
                    for (size_t i = 0; i < args.size(); ++i)
                    {
                        if (i != 0)
                            fprintf(fout, "  printf \", \"\n");
                        auto& arg = args[i];
                        if (arg == "long" || arg == "ptr")
                            fprintf(fout, "  printf \"0x%%X\", $p%d\n", (int)i);
                        else if (arg == "double")
                            fprintf(fout, "  printf \"%%f\", $p%d\n", (int)i);
                        else if (arg == "str")
                            fprintf(fout, "  ac_print $p%d\n", (int)i);
                        else if (arg == "wstr")
                            fprintf(fout, "  wc_print $p%d\n", (int)i);
                    }
                }
                fputs(
                    "  printf \")\\n\"\n"
                    "  cont\n"
                    "end\n"
                    "\n", fout);
            }

            fputs(
                "run\n"
                "quit\n", fout);

            fclose(fout);
            return 0;
        }
    }

    return 1;
}
