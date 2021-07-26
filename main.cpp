#include "TinyD.hpp"
#include "Utils.hpp"
#include "dat.hpp"

extern "C"
int wmain( int argc, wchar_t **argv)
{
    DoEnableProcessPriviledge(SE_DEBUG_NAME);
    std::setlocale(LC_CTYPE, "");

    TinyD td;

    td.AddForbidden("CharUpperW");
    td.AddForbidden("LoadIconW");
    td.AddForbidden("LoadCursorW");
    td.AddForbidden("CreateWindowExW");

    std::vector<DataEntry> entries;
    if (DAT_load("data.dat", entries))
    {
        td.SetDebugEntries(entries.size(), entries.data());
    }
    else
    {
        static const DataEntry s_entries[] =
        {
            { "user32", "stdcall", "MessageBoxA", { "ptr", "str", "str", "long" } },
            { "user32", "stdcall", "MessageBoxW", { "ptr", "wstr", "wstr", "long" } },
        };
        td.SetDebugEntries(_countof(s_entries), s_entries);
    }

    std::wstring cmdline = DoMakeCmdLine(argc, argv);
    if (!td.StartDebugProcess(cmdline.c_str()))
        return EXIT_FAILURE;

    return (INT)td.DebugLoop();
}

int main(int argc, char **argv)
{
    INT wargc;
    LPWSTR* wargv = CommandLineToArgvW(GetCommandLineW(), &wargc);
    INT ret = wmain(wargc, wargv);
    LocalFree(wargv);
    return ret;
}
