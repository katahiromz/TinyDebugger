#pragma once

#ifndef _INC_WINDOWS
    #include <windows.h>
#endif
#include <vector>
#include <string>

struct DataEntry
{
    std::string module_name;
    std::string cc;
    std::string func_name;
    std::vector<std::string> args;
};

bool DAT_load(const char *filename, std::vector<DataEntry>& entries);
bool DAT_save(const char *filename, const std::vector<DataEntry>& entries);
