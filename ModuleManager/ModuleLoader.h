#pragma once
#include <iostream>
#include <dlfcn.h>
#include <unordered_map>

#include "module.h"

class ModuleLoader
{
    public:
    ModuleLoader();    

    Module* load(const std::string& filename);

    void unload(const std::string& filename);

    private:
    std::unordered_map<std::string, void*> m_filename_handle_map;

};