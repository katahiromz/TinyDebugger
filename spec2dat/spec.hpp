#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <cstdio>
#include <cstdint>

struct SPEC_ITEM
{
    std::string module_name;
    std::string func_name;
    std::vector<std::string> args;
    std::string cc; // calling convension
};

struct SPEC
{
    std::unordered_map<std::string, SPEC_ITEM> func_name_to_item;
    std::set<std::string> modules;
};

bool SPEC_read(SPEC& spec, const char *module_name, FILE *fin);
