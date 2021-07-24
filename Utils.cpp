#include "Utils.hpp"

BOOL APIENTRY DoEnableProcessPriviledge(LPCWSTR pszSE_)
{
    BOOL f = FALSE;
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tp;

    if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        if (LookupPrivilegeValueW(NULL, pszSE_, &luid))
        {
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            tp.Privileges[0].Luid = luid;
            f = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);
        }
        CloseHandle(hToken);
    }
    return f;
}

BOOL APIENTRY DoReadWriteProcessMemory(HANDLE hProcess, LPVOID address,
                                       LPVOID data, DWORD data_size, BOOL bWrite)
{
    DWORD dwOldProtect;
    if (!VirtualProtectEx(hProcess, address, data_size, PAGE_EXECUTE_READWRITE, &dwOldProtect))
    {
        fprintf(stderr, "VirtualProtectEx failed\n");
        return FALSE;
    }

    BOOL ret;
    if (bWrite)
        ret = WriteProcessMemory(hProcess, address, data, data_size, NULL);
    else
        ret = ReadProcessMemory(hProcess, address, data, data_size, NULL);

    VirtualProtectEx(hProcess, address, data_size, dwOldProtect, NULL);

    return ret;
}

std::wstring APIENTRY DoMakeCmdLine(int argc, wchar_t **argv)
{
    std::wstring cmdline;
    for (int i = 1; i < argc; ++i)
    {
        if (i > 1)
            cmdline += L" ";
        std::wstring arg = argv[i];
        if (arg.find_first_of(L" \t") != arg.npos)
        {
            cmdline += L'\"';
            cmdline += arg;
            cmdline += L'\"';
        }
        else
        {
            cmdline += arg;
        }
    }
    return cmdline;
}
