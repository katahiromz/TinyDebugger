#pragma once

#ifndef _INC_WINDOWS
    #include <windows.h>
#endif
#include <string>

BOOL APIENTRY DoEnableProcessPriviledge(LPCWSTR pszSE_);
BOOL APIENTRY DoReadWriteProcessMemory(HANDLE hProcess, LPVOID address,
                                       LPVOID data, DWORD data_size, BOOL bWrite);
std::wstring APIENTRY DoMakeCmdLine(int argc, wchar_t **argv);
