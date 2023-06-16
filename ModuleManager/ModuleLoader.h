#ifndef MODULE_LOADER_H
#define MODULE_LOADER_H

#include <dlfcn.h>
#include <string>
#include <memory>
#include <unordered_map>

#include "../Module/Module.h"

class ModuleLoader
{    
    public:
    using ModulePtr = std::shared_ptr<Module>;

    ModuleLoader();    

    ModulePtr load(const std::string& filename);

    void unload(const std::string& filename);

    private:
    std::unordered_map<std::string, void*> m_filename_handle_map;

};

#endif